#include<unistd.h>
#include<sys/mman.h>
#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>

/*
TS-7800 SPI bus routines.  Usage:
First, with no SPI devices selected, call init_spi()
Then assert the chip select for your SPI device.
Then, you can use spi8 or spi32 to write and read. (normal full-duplex SPI operation -- spi8(x) sends x and then
returns any data the the slave device was sending.)
spi8 works with char values, spi32 with int values.
spi_write8 and spi_write32 are faster by a factor of 5 because they send data but ignore responses from the slave device.
*/


#define SPI_DIOBASE 0xE8000000
#define SPI_CLK (1 << 5)
#define SPI_MOSI (1 << 3)
#define SPI_MISO (1 << 1)

#define SPI_RO *(spi_dioptr + 0x05/sizeof(unsigned char))
#define SPI_RW *(spi_dioptr + 0x09/sizeof(unsigned char))

volatile unsigned char *spi_dioptr;

void init_spi() {
  int fd;
  fd = open("/dev/mem", O_RDWR|O_SYNC);
  spi_dioptr = (unsigned char *)mmap(0, getpagesize(),
    PROT_READ|PROT_WRITE, MAP_SHARED, fd, SPI_DIOBASE);
  SPI_RW |= SPI_CLK;
}

unsigned char spi8(unsigned char c) {
  int i;
  unsigned char m0c0, m1c0, m0c1, m1c1;
  unsigned char ret = 0;
  
  m1c1 = SPI_RW | SPI_MOSI | SPI_CLK;
  m1c0 = m1c1 & ~SPI_CLK;
  m0c0 = m1c0 & ~SPI_MOSI;
  m0c1 = m1c1 & ~SPI_MOSI;
  for(i=0; i < 8; i++) {
    if (c & 0x80) {
      SPI_RW = m1c1;
      ret <<= 1;  
      SPI_RW = m1c0;
      c <<= 1;
      if (~SPI_RO & SPI_MISO) ret |= 1; // SPI MISO comes out inverted on the 7800.
    } else {
      SPI_RW = m0c1;
      ret <<= 1;  
      SPI_RW = m0c0;
      c <<= 1;
      if (~SPI_RO & SPI_MISO) ret |= 1; // SPI MISO comes out inverted on the 7800.
    }
  }
  SPI_RW = m0c1;
  return ret;
}

unsigned int spi32(unsigned int c) {
  int i;
  unsigned char m0c0, m1c0, m0c1, m1c1;
  unsigned int ret = 0;
  
  m1c1 = SPI_RW | SPI_MOSI | SPI_CLK;
  m1c0 = m1c1 & ~SPI_CLK;
  m0c0 = m1c0 & ~SPI_MOSI;
  m0c1 = m1c1 & ~SPI_MOSI;
  for(i=0; i < 32; i++) {
    if (c & 0x80000000) {
      SPI_RW = m1c1;
      ret <<= 1;  
      SPI_RW = m1c0;
      if (~SPI_RO & SPI_MISO) ret |= 1; // SPI MISO comes out inverted on the 7800.
      c <<= 1;
    } else {
      SPI_RW = m0c1;
      ret <<= 1;  
      SPI_RW = m0c0;
      if (~SPI_RO & SPI_MISO) ret |= 1; // SPI MISO comes out inverted on the 7800.
      c <<= 1;
    }
  }
  SPI_RW = m0c1;
  return ret;
}

void spi_write8(unsigned char c) {
  int i;
  unsigned char m0c0, m1c0, m0c1, m1c1;
  
  m1c1 = SPI_RW | SPI_MOSI | SPI_CLK;
  m1c0 = m1c1 & ~SPI_CLK;
  m0c0 = m1c0 & ~SPI_MOSI;
  m0c1 = m1c1 & ~SPI_MOSI;
  for(i=0; i < 8; i++) {
    if (c & 0x80) {
      SPI_RW = m1c1;
      c <<= 1;
      SPI_RW = m1c0;
    } else {
      SPI_RW = m0c1;
      c <<= 1;
      SPI_RW = m0c0;
    }
  }
  SPI_RW = m0c1;
}

void spi_write32(unsigned int c) {
  int i;
  unsigned char m0c0, m1c0, m0c1, m1c1;
  
  m1c1 = SPI_RW | SPI_MOSI | SPI_CLK;
  m1c0 = m1c1 & ~SPI_CLK;
  m0c0 = m1c0 & ~SPI_MOSI;
  m0c1 = m1c1 & ~SPI_MOSI;
  for(i=0; i < 32; i++) {
    if (c & 0x80000000) {
      SPI_RW = m1c1;
      c <<= 1;
      SPI_RW = m1c0;
    } else {
      SPI_RW = m0c1;
      c <<= 1;
      SPI_RW = m0c0;
    }
  }
  SPI_RW = m0c1;
}

