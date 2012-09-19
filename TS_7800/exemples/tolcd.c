/*
  TS-7800 LCD sample code
  2007-1-8:mos:initial version

  Important Notes:
  1. The TS-7800 bus has a FIFO for bus transactions.  If you care about
     order in which things happen on this bus, you must read back the
     memory address you are writing to force the transaction to flush.
     Old MacDonald had a bus, EIEIO.
  2. The pin-out of the LCD header is the mirror(*) image of the pin-out
     of the actual LCD.  The table below illustrates how this works.
  3. Performance could be optimized by reducing delay times.

  (*)Why does RAID-1 claim to "mirror" your drive when the bits aren't
  written backwards?
*/

#include<unistd.h>
#include<sys/types.h>
#include<sys/mman.h>
#include<sys/time.h>
#include<stdio.h>
#include<fcntl.h>
#include<string.h>
#include<stdlib.h>

#define DIOBASE       0xE8000000
#define RW_MASK       0x00200000
#define ENABLE_MASK   0x00100000
#define BASELINE_MASK 0x00080000
#define RS_MASK       0x00040000
#define BUSY_MASK     0x20000000

#define RW_REG(ptr) *(ptr + 0x08/sizeof(unsigned int))

/*
 HD44780 character based LCD

 Pin Assignments
 ---------------------------------
 1     +5V
 2     GND
 3     contrast
 4 I   RS (0=instruction, 1=data)
 5 I   RW (0=write, 1=read)
 6 I   E  (enable)
 7 I/O DB0 (data bus line 0, lsb)
 8 I/O DB1 ...
 9 I/O DB2
10 I/O DB3
11 I/O DB4
12 I/O DB5
13 I/O DB6
14 I/O DB7 (data bus line 7, msb)

 Mirrored Pin Assignments
 ---------------------------------
 1     GND
 2     +5V
 4     contrast
 3 I   RS (0=instruction, 1=data)
 6 I   RW (0=write, 1=read)
 5 I   E  (enable)
 8 I/O DB0 (data bus line 0, lsb)
 7 I/O DB1 ...
10 I/O DB2
 9 I/O DB3
12 I/O DB4
11 I/O DB5
14 I/O DB6
13 I/O DB7 (data bus line 7, msb)

 */
volatile unsigned int *lcd;

#define MIRROR_PINS

inline int getLCDbit(int n) {
#ifdef MIRROR_PINS
  switch (n) {
  case 3: n =4 ; break;
  case 4:  n =3 ; break;
  case 5: n =6 ; break;
  case 6: n =5 ; break;
  case 7: n =8 ; break;
  case 8: n =7 ; break;
  case 9: n =10 ; break;
  case 10: n =9 ; break;
  case 11: n =12 ; break;
  case 12: n =11 ; break;
  case 13: n =14 ; break;
  case 14: n =13 ; break;
 }
#endif
  return (*lcd >> (16+n-1)) & 1;
}

//inline
void putLCDbit(int n,int val) {
   volatile int i;
   unsigned int tmp;

#ifdef MIRROR_PINS
  switch (n) {
  case 3: n =4 ; break;
  case 4:  n =3 ; break;
  case 5: n =6 ; break;
  case 6: n =5 ; break;
  case 7: n =8 ; break;
  case 8: n =7 ; break;
  case 9: n =10 ; break;
  case 10: n =9 ; break;
  case 11: n =12 ; break;
  case 12: n =11 ; break;
  case 13: n =14 ; break;
  case 14: n =13 ; break;
 }
#endif
  if (val) {
    tmp = *(lcd+1);
    tmp |= (1 << (16+n-1));
    *(lcd+1) = tmp;
  } else {
    tmp = *(lcd+1);
    tmp &= ~(1 << (16+n-1));
    *(lcd+1) = tmp;
  }
  i = *(lcd+1); // force bus cycle to flush write now
  if (i != tmp) printf("put %X, got %X\n",tmp,i); // don't really need this
}

inline void lcd_instr(int i) {
  putLCDbit(4,!i);
}

inline void lcd_data_out(int data) {
  int i;
  for (i=0;i<8;i++) {
    putLCDbit(i+7,data & 1);
    data >>= 1;
  }
}

inline void lcd_write(int w) {
  if (!w) {
    lcd_data_out(0xFF);
  }
  putLCDbit(5,!w);
}

inline void lcd_enable(int e) {
  putLCDbit(6,e);
}

inline void lcd_wait_not_busy() {
  int i = 1000,val;
  volatile int ii;

  do {
    lcd_instr(1);
    lcd_write(0);
    for (ii=0;ii<10000;ii++); // wait 800nS minimum
    lcd_enable(1);
    for (ii=0;ii<10000;ii++); // wait 800nS minimum
    //usleep(10000);
    val = getLCDbit(14);
    lcd_enable(0);
    for (ii=0;ii<10000;ii++); // wait 800nS minimum
  } while (--i && val);
  if (i == 0) {
    printf("LCD timeout %X\n",val);
  }
}

inline void lcd_cmd(int cmd) {
  volatile int i;

  lcd_write(1);
  lcd_data_out(cmd);
  lcd_instr(1);
  for (i=0;i<1000;i++);
  lcd_enable(1);
  //usleep(10000);
  for (i=0;i<10000;i++);
  lcd_enable(0);
  lcd_wait_not_busy();
}

inline void lcd_data(int data) {
  volatile int i;

  lcd_data_out(data);
  lcd_instr(0);
  lcd_write(1);
  for (i=0;i<1000;i++);
  lcd_enable(1);
  for (i=0;i<10000;i++); // wait 800nS minimum
  //usleep(10000);
  lcd_enable(0);
  lcd_wait_not_busy();
}

inline void lcd_cmd_cls() {
  lcd_cmd(0x01);
}


// first argument is printed on the first line
// second argument is printed on the second line
//
int main(int argc,char *argv[]) {
  volatile unsigned int *dioptr;
  int fd = open("/dev/mem", O_RDWR|O_SYNC);
  char *s;

  dioptr = (unsigned int *)mmap(0, getpagesize(),
    PROT_READ|PROT_WRITE, MAP_SHARED, fd, DIOBASE);
  RW_REG(dioptr) &= ~BASELINE_MASK; /* baseline should always be zero */

  lcd = dioptr + 1;
  
  lcd_cmd(0x38);
  lcd_cmd(0x38);
  lcd_cmd(0x38);
  lcd_cmd(0x6);
  lcd_cmd_cls();
  lcd_cmd(0xC);
  lcd_cmd(0x2);
  
  if (argc < 2) return 0;
  s = argv[1];
  while (*s) {
    lcd_data(*(s++));
  }
  if (argc > 2) {
    lcd_wait_not_busy();
    lcd_cmd(0xa8); // // set DDRAM addr to second row
    s = argv[2];
    while (*s) {
      lcd_data(*(s++));
    }
  }
  return 0;
}

// References:
// http://home.iae.nl/users/pouweha/lcd/lcd0.shtml#hd44780
// describes how to talk to the HD44780 character LCD display.
