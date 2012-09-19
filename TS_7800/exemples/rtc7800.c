#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <time.h>
#include <unistd.h>
#include <sys/mman.h>

int tobcd(int n) {
  int ret = 0, i = 0;

  while (n) {
    ret |= (n % 10) << (i * 4);
    n = n / 10;
    i++;
  }
  return ret;
}

int frombcd(int n) {
  int ret = 0, i = 1;

  while(n) {
    ret += (n & 0xf) * i;
    i = i * 10;
    n = n >> 4;
  }
  return ret;
}

#define wRTC(addr,dat) *adrs = addr; *data = dat;
#define rRTC(addr) (*adrs = addr, *data)

int main(int argc, char **argv) {
  unsigned char *adrs,*data;
  unsigned char *start;
  time_t now;

  if (argv[1] && strcmp(argv[1],"-s") && strcmp(argv[1],"-t")) {
    printf("Usage: rtc7800 [-s][-t]\n");
    printf("With no options, transfer RTC time to system.\n");
    printf("With -s option (\"save to RTC\"), transfers system time to RTC.\n");
    printf("With -t option (\"time check\"), only displays RTC time\n");
    printf("The options are mutually exclusive (only the first option will be honored)\n");
  }

  int fd = open("/dev/mem", O_RDWR|O_SYNC);
  start = mmap(0, getpagesize(), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0xe8000000);
  if (start == MAP_FAILED) {
    perror("mmap:");
    return 1;
  }
  adrs = start + 0x808;
  data = start + 0x80C;

  wRTC(127,0x55);
  wRTC(126,0x00);
  if (rRTC(127) == 0x55) {
    wRTC(127,0xAA);
    if (rRTC(127) == 0xAA && rRTC(126) == 0) {
      goto rtcok;
    }
  }
  printf("TS-7800 RTC not detected\n");
  return 1;

 rtcok:
  wRTC(0xA,0x20);  // magic knock
  wRTC(0xB,rRTC(0xB) | 0x02);  // 24-hour mode
  if (argv[1] && strcmp(argv[1], "-s") == 0) { // set RTC from system time
    struct tm *t;
    int count = 1000000;

    time(&now);
    t = localtime(&now);

    assert(t->tm_year + 1900 >= 2007);
    while (count-- && (rRTC(0xA) & 0x80)); // wait if not accessible yet
    if (count == 0) {
      fprintf(stderr,"Timeout waiting for RTC\n");
      return 1;
    }
    wRTC(0xB,rRTC(0xB) | 0x80);  // disable update
    wRTC(0x0,tobcd(t->tm_sec));
    wRTC(0x2,tobcd(t->tm_min));
	printf("Hour=%02X (%d) (dec=%d)\n",tobcd(t->tm_hour),tobcd(t->tm_hour),t->tm_hour);
    wRTC(0x4,tobcd(t->tm_hour));
	printf("Read=%02X (%d)\n",rRTC(0x4),rRTC(0x4));
    wRTC(0x6,tobcd(t->tm_wday));
    wRTC(0x7,tobcd(t->tm_mday));
    wRTC(0x8,tobcd(t->tm_mon + 1));
    wRTC(0x9,tobcd(t->tm_year - 100));

    wRTC(0xB,rRTC(0xB) & ~0x80); // re-enable update
  } else { // set system time from RTC
    struct tm t;
    struct timeval tv;
    int h,pm;

    wRTC(0xB,rRTC(0xB) | 0x80);  // NEW: disable update
    t.tm_sec = frombcd(rRTC(0x0));
    t.tm_min = frombcd(rRTC(0x2));
    h = rRTC(0x4) & 0x7F;
    t.tm_hour = frombcd(h);
    t.tm_mday = frombcd(rRTC(0x7));
    t.tm_mon = frombcd(rRTC(0x8)) - 1;
    t.tm_year = frombcd(rRTC(0x9)) + 100;
    t.tm_isdst = -1;
    wRTC(0xB,rRTC(0xB) & ~0x80); // NEW: re-enable update

    if (argv[1] && strcmp(argv[1], "-t") == 0) { // only display RTC time
      printf("RTC time: %02d-%02d-%04d %02d:%02d:%02d\n",
	     t.tm_mon+1,t.tm_mday,t.tm_year+1900,t.tm_hour,t.tm_min,t.tm_sec);
    } else {
      now = mktime(&t);
      tv.tv_sec = now;
      tv.tv_usec = 0;
      assert(settimeofday(&tv, NULL) == 0);
    }
  }

  close(fd);
  return 0;
}
