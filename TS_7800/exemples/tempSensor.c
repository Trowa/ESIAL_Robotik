#include<unistd.h>
#include<sys/mman.h>
#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>

#include "spi.c"

/*
Technologic systems TS-7800 temperature sensor.
Updated to utilize spi.c.
*/

#define DIOBASE 0xE8000000
#define CS_PIN 31

#define RW_REG(ptr) *(ptr + 0x08/sizeof(unsigned int))

float tempC(unsigned int data);
float CtoF(float x);

int main() {
  unsigned int rawtemp = 0;
  volatile unsigned int *dioptr;
  int fd = open("/dev/mem", O_RDWR|O_SYNC);
  dioptr = (unsigned int *)mmap(0, getpagesize(),
    PROT_READ|PROT_WRITE, MAP_SHARED, fd, DIOBASE);
  
  RW_REG(dioptr) &= ~(1 << CS_PIN); // make sure temp sensor not chip selected
  init_spi(); // call init_spi before selecting an SPI device
  RW_REG(dioptr) ^= (1 << CS_PIN); // temp sensor chip select
  rawtemp = spi8(0) << 8;
  rawtemp |= spi8(0);
    
/*  printf("%X\n", tempdata(dioptr)); */
  printf("The temperature in Fahrenheit is %f\n", CtoF(tempC(rawtemp)));
  
  return 0;
  }

float tempC(unsigned int data) {
  float ret;
  if (data & (1 << 15)) {  /* negative temperatures: */
    ret = -0.0625 * (~((data >> 3) | 0xFFFFE000) + 1);
    } else {               /* positive temperatures: */
    ret = .0625 * (data >> 3);  /* 3 final bits are irrelevant */
    }
  return ret;
  }
    
float CtoF(float x) {
  return x*9/5 + 32;
  }


