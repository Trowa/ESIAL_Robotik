#include<unistd.h>
#include<sys/types.h>
#include<sys/mman.h>
#include<sys/time.h>
#include<stdio.h>
#include<fcntl.h>
#include<string.h>
#include <stdlib.h>

#define DIOBASE 0xE8000000
#define PINMASK(x) (1 << (x << 1))

#define KEY_0 2
#define KEY_1 5
#define KEY_2 6
#define KEY_3 7
#define KEY_4 9
#define KEY_5 10
#define KEY_6 11
#define KEY_7 13
#define KEY_8 14
#define KEY_9 15
#define KEY_UP 8
#define KEY_DOWN 12
#define KEY_2ND 16
#define KEY_CLEAR 1
#define KEY_HELP 3
#define KEY_ENTER 4

/* ************************************************************************
 This simple program demonstrates input to the TS-7800 from the matrix keypad.
 **************************************************************************
*/

int printkey(unsigned int key);
unsigned int getkey(volatile unsigned int *dioptr);
int accept_keypress(volatile unsigned int *dioptr);

int main() {
  unsigned int key;
  volatile unsigned int *dioptr;
  int fd = open("/dev/mem", O_RDWR|O_SYNC);
  
  dioptr = (unsigned int *)mmap(0, getpagesize(),
    PROT_READ|PROT_WRITE, MAP_SHARED, fd, DIOBASE);
  
  printf("Press some keys, or enter to quit:\n");
  do {
    key = (accept_keypress(dioptr));
    printkey(key);
    } while (key != KEY_ENTER);
    
  return 0;
  }  


/* accept_keypress does not return until a key is pressed and released. */
int accept_keypress(volatile unsigned int *dioptr) {
  unsigned int ret = 0, counter = 0;
  
  while (ret == 0) {
/*    usleep(10); */
    ret = getkey(dioptr);
    }
  do {
    counter++;
    if (getkey(dioptr) == ret) {
      counter = 0;
      }
    usleep(1);
    } while (counter < 10);
  return ret;
  }

/* getkey checks the inputs once and returns 0 for no key or 1 to 16 for a keypress */
unsigned int getkey(volatile unsigned int *dioptr) {

  unsigned int i1, i2, response, ret = 0;

  for(i1 = 0; i1 < 4; i1++) {
    *(dioptr + 0x08/sizeof(unsigned int)) = 0x5555u ^ PINMASK(i1);    /* write to all pins but one */
    response = 0x5500u ^ (0x5500u & *(dioptr + 0x04/sizeof(unsigned int)));    /* filter out zeros in output */
    if (response) {       /* response is zero if no keys on the current row */
      for(i2 = 0; i2 < 4; i2++) {
        if(PINMASK(i2) & (response >> 8)) { ret = 4 * i1 + i2 + 1;}   
        }
      }
    }
  return ret;
  }

int printkey(unsigned int key) {
  switch (key) {
    case KEY_CLEAR:
      printf("clear");
      break;
    case KEY_0:
      printf("0");
      break;
    case KEY_HELP:
      printf("help");
      break;
    case KEY_ENTER:
      printf("enter");
      break;
    case KEY_1:
      printf("1");
      break;
    case KEY_2:
      printf("2");
      break;
    case KEY_3:
      printf("3");
      break;
    case KEY_UP:
      printf("up arrow");
      break;
    case KEY_4:
      printf("4");
      break;
    case KEY_5:
      printf("5");
      break;
    case KEY_6:
      printf("6");
      break;
    case KEY_DOWN:
      printf("down arrow");
      break;
    case KEY_7:
      printf("7");
      break;
    case KEY_8:
      printf("8");
      break;
    case KEY_9:
      printf("9");
      break;
    case KEY_2ND:
      printf("2nd");
      break;
    default:
      printf("nothing");
      break;
    }
  printf("\n");
  return 0;
  }
