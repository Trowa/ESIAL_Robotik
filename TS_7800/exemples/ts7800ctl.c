#include <assert.h> 
#include <fcntl.h>
#include <sched.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include "twsi.h"


volatile unsigned int *data, *control, *status, *led;
unsigned int verbose = 0, addr;

static void exit_gracefully() {
	fprintf(stderr, "EXITING....\n");
	*control = STOP|TWSIEN; // Send stop signal
	exit(0);
}

static int parsechans(const char *str, unsigned int len) {

	int chans=0, last_ch=-1, dash=0, i, j;
	//Determine which channels to sample
	for(i=0;i<len;i++) {
		if((str[i] >= '0' && str[i] <= '3') ||
		  (str[i] >= '6' && str[i] <= '7')) {
			if(dash) {
				dash = 0;
				if(last_ch < 0) {
					printf("Invalid format, Sample ADC channels CHANS, e.g. \"1-3,5\", \"2,3,5\"\n");
					printf("\tCh | Pin\n");
					printf("\t---+----\n");
					printf("\t 0 | 1\n");
					printf("\t 1 | 3\n");
					printf("\t 3 | 5\n");
					printf("\t 2 | 7\n");
					printf("\t 7 | 9\n");

					return -1;
				}
		
				for(j=last_ch; j<=(str[i]-'0'); j++)
					chans |= (1<<j);		

			} else {
				last_ch = str[i] - '0';
				chans |= 1<<(str[i] - '0');
			}

		} else if(str[i] == '-') { dash=1;
		} else if((str[i] == ',') || (str[i] == ' ')){ ; 
		} else {
			printf("Invalid format, Sample ADC channels CHANS, e.g. \"1-3,5\", \"2,3,5\"\n");
			printf("\tCh | Pin\n");
			printf("\t---+----\n");
			printf("\t 0 | 1\n");
			printf("\t 1 | 3\n");
			printf("\t 3 | 5\n");
			printf("\t 2 | 7\n");
			printf("\t 7 | 9\n");
			return -1;
		}
	}

	//channel 6 => bit 4
	if(chans & (1<<6)) {
		chans |= 1<<4;
		chans &= ~(1<<6);
	}

	//channel 7 => bit 5
	if(chans & (1<<7)) {
		chans |= 1<<5;
		chans &= ~(1<<7);
	}

	return chans;
}

static void usage(char **argv) {
        fprintf(stderr, "Usage: %s [OPTION] ...\n"
	  "Modify state of TS-7800 hardware.\n"
	  "\n"
	  "General options:\n"
	  "  -s    seconds         Number of seconds to sleep for\n"
	  "  -f                    Feed the WDT for 8s\n"
	  "  -d                    Disable the WDT\n"
	  "  -r    CHANS           Sample ADC channels CHANS, e.g. \"1-3,5\", \"2,3,5\"" 
                                  "output raw data to standard out\n"
          "  -S    CHANS           Sample ADC channels CHANS, e.g. \"1-3,5\", \"2,3,5\"" 
                                  "output string parseable data to standard out\n"
	  "  -A    ADDR            Write DATA to ADDR in one time programmable memory\n"
	  "  -D    DATA            Write DATA to ADDR in one time programmable memory\n"
	  "  -n                    Red LED on\n"
	  "  -F                    Red LED off\n"
          "  -o                    Display one time programmable data\n"
          "  -m                    Display contents of non-volatile memory\n"
          "  -M                    Display MAC address\n"
          "  -O                    Display odometer(hrs board has been running for)\n"
          "  -B                    Display birthdate\n"     
	  "  -V                    Verbose output\n"			
	  "  -h                    This help screen\n",argv[0]);
}

int static inline twi_write(unsigned char dat) {
	volatile int i;
	unsigned int s;

	*data = dat;
	for(i = 0; i < 60; i++);

	*control = ACK;
	for(i = 0; i < 60; i++);

	while((*control & IFLG) == 0x0);        // Wait for an I2C event

	s = *status;
	assert((s == ST_DATA_ACK) || (s == MT_DATA_ACK )); 

	return 0;
}
	
unsigned char static inline twi_read() {
	volatile int i;
	unsigned int s;

	*control = (*control & ~IFLG) | ACK;
	for(i = 0; i < 60; i++);

	while((*control & IFLG) == 0x0);	// Wait for an I2C event 
	s = *status;
	if((s != SR_DATA_ACK) && (s != MR_DATA_ACK)) return -1;

	return (unsigned char)*data;
}

void static inline twi_select(unsigned char addr, unsigned char dir) {
	volatile int i;
	unsigned int s;

begin:

	*control = START|TWSIEN;        // Send start signal
	for(i = 0; i < 1000; i++);

	while((*control & IFLG) == 0);

	switch (*status) {
		case MT_START:
		case MT_REP_START:
			break;
		default:	// retry forever
			goto begin;
	}

	*data = addr|dir;     // Send SLA+W/R

	/* XXX: The following for() delay is ABSOLUTELY NECESSARY otherwise
	 * the Marvel TWSI controller shifts out wrong data as the SLA+W.
	 * There is no official documentation or errata regarding this but
	 * there is a clue in Marvell supplied TWSI Linux code: 
	 * arch/arm/mach-mv88fxx81/Soc/mvTwsi.c line 276 after it adds an 
	 * arbitrary 1ms delay with a comment: "Wait 1 milli to prevent TWSI
	 * write after write problems"
	 *  
	 * The below for() loop however was the only one that seemed
	 * necessary for proper operation while testing.  However, after
	 * discovering that comment of code, delays were added after EVERY
	 * TWSI register write.  
	 *
	 * The number 60 is precisely 2x the required number of 30
	 * discovered during testing.  --joff
	 */
	for(i = 0; i < 60; i++);

	*control = ACK|TWSIEN;
	for(i = 0; i < 60; i++);

	while ((*control & IFLG) == 0);
	s = *status;
	assert((s == MT_SLA_ACK) || (s == MR_SLA_ACK)); 
}

volatile unsigned int adcfifo_put = 0, adcfifo_get = 0, adc_continue;
unsigned short adcfifo[65536];

void service_adc(int x) {
	static unsigned int pos = 0;
	unsigned int s, put = adcfifo_put, get = adcfifo_get;
	volatile int y;

	twi_select(AVR_ADDR, READ);

	for (;;) {
		s = twi_read() << 8;
		if (s == 0xff00) break;
		s |= twi_read();
		if (pos == 512) adc_continue = 1;
		pos = (pos + 1) & 0x3ff;
		if (((put - get) & 0xffff) < 65534) {
			adcfifo[put++] = s;
			put &= 0xffff;
		}
	} 

	*control = STOP|TWSIEN; 	//Send stop signal
	for (y = 0; y < 2500; y++);

	adcfifo_put = put;
}


int main(int argc, char **argv) {
	int c, devmem;
	unsigned int otp_addr, otp_data, secs = 0;
	unsigned int val_addr=0, val_data=0, *twi_regs, *regs;
	unsigned int start_adc=0, raw=0;
	unsigned int display_odom=0, did_something=0, display_bday=0;
	unsigned int display_otp=0, display_mem=0, display_mac=0;
	unsigned int len, odom, bday;
	unsigned char str[80];
	volatile int y;

	signal(SIGTERM, exit_gracefully);
	signal(SIGHUP, exit_gracefully);
	signal(SIGINT, exit_gracefully);
	signal(SIGPIPE, exit_gracefully);

	if(argc == 1) {
		usage(argv);
		return 1;
	}

	devmem = open("/dev/mem", O_RDWR|O_SYNC);
	assert(devmem != -1);
	twi_regs = (unsigned int *)mmap(0, getpagesize(), PROT_READ|PROT_WRITE,
	  MAP_SHARED, devmem, 0xF1011000);

	regs = (unsigned int *)mmap(0, getpagesize(), PROT_READ|PROT_WRITE,
	  MAP_SHARED, devmem,  0xe8000000);

	led = &regs[GLED];
        control = &twi_regs[CONTROL];
        data = &twi_regs[DATA];
        status = &twi_regs[STATUS];

	while ((c = getopt(argc, argv, "s:fdr:S:A:D:nFVoOmMB")) != -1) {

		switch(c) {
			case 's':
				secs = strtoul(optarg, NULL, 0);
				if((secs > 0) && (secs <= (65535 * 8))){
					if((secs % 8) > 0)  secs=(secs/8)+1;
					else secs=(secs/8);
				} else if(secs > (65535 * 8))
					printf("Invalid sleep time,"
					  "maximum sleep time is 524288\n");
				did_something=1;
				break;
		
			case 'f':
				twi_select(AVR_ADDR, WRITE);
				twi_write(WDT_8s);
				*control = STOP|TWSIEN; // Send stop signal
				if(verbose) fprintf(stderr,"Sent Stop signal\n");
				did_something=1;
				break;

			case 'd':
				twi_select(AVR_ADDR, WRITE);
				twi_write(WDT_OFF);
				*control = STOP|TWSIEN; // Send stop signal
				if(verbose) fprintf(stderr,"Sent Stop signal\n");
				did_something=1;
				break;

			case 'r':
				start_adc=1;
				raw=1;
				for(len=0;len<80;len++) 
					if(optarg[len]=='\0') break;

				strncpy(str, optarg, len);
				str[len] = '\0';
				break;

			case 'S':
				start_adc=1;
				for(len=0;len<80;len++)
					if(optarg[len]=='\0') break;
				strncpy(str, optarg, len);
				str[len] = '\0';
				break;
			
			case 'A':
				otp_addr = strtoul(optarg, NULL, 0);
				if(otp_addr < 256) val_addr = 1;
				else fprintf(stderr, "Invalid address," 
				  " valid address are 0-63\n");
				break;

			case 'D':
				otp_data = strtoul(optarg, NULL, 0);
				if(otp_data < 256) val_data = 1;
				else fprintf(stderr, "Invalid data,"
				  " valid data is 0-255");
				break;
			
			case 'n':
				twi_select(AVR_ADDR, WRITE);
				twi_write(LED_ON);
				*control = STOP|TWSIEN; // Send stop signal
				did_something=1;
				break;

			case 'F':
				twi_select(AVR_ADDR, WRITE);
				twi_write(LED_OFF);
				*control = STOP|TWSIEN; // Send stop signal
				did_something=1;
				break;

			case 'V':
				verbose = 1;
				break;

			case 'M':
				display_mac = 1;
				did_something=1;
				break;

			case 'O':
				display_odom = 1;
				did_something=1;
				break;

			case 'B':
				display_bday = 1;
				did_something=1;
				break;

			case 'o':
				display_otp = 1;
				did_something=1;
				addr=6;
				break;

			case 'm':
				display_mem = 1;
				did_something=1;
				addr=70;
				break;

			case 'h':	
			default:
				usage(argv);
				return 1;
		}
	}

	if (start_adc) {
		struct itimerval it;
		struct sigaction sa;
		struct sched_param sched;
		sigset_t sigs;
		unsigned int seq=0;
		int chans, i;
		adc_continue = did_something = 1;

		chans = parsechans(str, len);
		if(chans < 0) return -1;

		twi_select(AVR_ADDR, WRITE);
		twi_write(OTP_R);
		twi_write(157);
		*control = STOP|TWSIEN; 	//Send stop signal

		bzero(&sa, sizeof(sa));
		sa.sa_handler = service_adc;
		sa.sa_flags = SA_RESTART;
		sigemptyset(&sa.sa_mask);
		sigaction(SIGALRM, &sa, NULL);
		sigemptyset(&sigs);
		sigaddset(&sigs, SIGALRM);
		it.it_interval.tv_sec = it.it_value.tv_sec = 0;
		it.it_interval.tv_usec = it.it_value.tv_usec = 10000;
		setitimer(ITIMER_REAL, &it, NULL);
		mlockall(MCL_CURRENT|MCL_FUTURE);
		sched.sched_priority = 50;
		sched_setscheduler(0, SCHED_FIFO, &sched);


adcloop:
		while (adcfifo_get != adcfifo_put) {
			unsigned short s = adcfifo[adcfifo_get];
			adcfifo_get = (adcfifo_get + 1) & 0xffff;
			if (raw) {
				putchar(s);
				putchar(s >> 8);
			} else printf("[0x%08x, %1d]=%04u\n", seq++, 
			  s >> 12, s & 0xfff);
		}		

		if (adc_continue) do {
			sigprocmask(SIG_BLOCK, &sigs, NULL);

			adc_continue = 0;
			twi_select(AVR_ADDR, WRITE);
			i = twi_write(chans | 0x40);
			*control = STOP|TWSIEN; // Send stop signal
			for (y = 0; y < 2500; y++);
			sigprocmask(SIG_UNBLOCK, &sigs, NULL);
		} while (i < 0); 

		pause();
		if (!feof(stdout) && !ferror(stdout)) goto adcloop;
	}

	if(secs > 0) {
		twi_select(AVR_ADDR, WRITE);
		twi_write(SLEEP);
		twi_write((secs >> 8) & 0xFF);	//MSB
		twi_write(secs & 0xFF);		//LSB
		*control = STOP|TWSIEN; 	//Send stop signal
		if(verbose) fprintf(stderr,"Sent Stop signal\n");
	}
	
	if(val_data && val_addr) {

		did_something=1;
		printf("OTP: writing 0x%x to 0x%x\n", otp_data, otp_addr);	
		twi_select(AVR_ADDR, WRITE);
		twi_write(OTP_W);
		twi_write(otp_addr);
		twi_write(otp_data);
		*control = STOP|TWSIEN; 	//Send stop signal
		if(verbose) fprintf(stderr,"Sent Stop signal\n");
	}

	if(display_odom) {

		twi_select(AVR_ADDR, WRITE);
		twi_write(OTP_R);
		twi_write(134);
		*control = STOP|TWSIEN; 	//Send stop signal
		for (y = 0; y < 2500; y++);
		twi_select(AVR_ADDR, READ);
		odom = (twi_read() << 24);
		odom |= (twi_read() << 16);
		odom |= (twi_read() << 8);
		odom |= twi_read();	
		*control = STOP|TWSIEN; 	//Send stop signal
printf("odom=0x%x\n", odom&0xFFFFFF);
		if(verbose) fprintf(stderr,"SENT Stop signal\n");

		odom = 0xFFFFFF - (odom & 0xFFFFFF);
		printf("TS-7800 has been running for %u hours\n", odom);	
	}

	if(display_bday) {

		twi_select(AVR_ADDR, WRITE);
		twi_write(OTP_R);
		twi_write(145);
		twi_select(AVR_ADDR, READ);
		bday = (twi_read() << 24);
		bday |= (twi_read() << 16);
		bday |= (twi_read() << 8);
		bday |= twi_read();	
		*control = STOP|TWSIEN; 	//Send stop signal

		if(verbose) fprintf(stderr,"SENT Stop signal\n");
		printf("TS-7800 was born on %02d/%02d/%04d\n", (bday>>24), (bday>>16)&0xFF, bday&0xFFFF);	
	}

	if(display_mac) {

		unsigned char mac[6];
		twi_select(AVR_ADDR, WRITE);
		twi_write(OTP_R);
		twi_write(0);
		twi_select(AVR_ADDR, READ);
		mac[5] = twi_read();
		mac[4] = twi_read();
		mac[3] = twi_read();
		mac[2] = twi_read();	
		*control = STOP|TWSIEN; 	//Send stop signal
		if(verbose) fprintf(stderr,"SENT Stop signal\n");

		twi_select(AVR_ADDR, WRITE);
		twi_write(OTP_R);
		twi_write(4);
		twi_select(AVR_ADDR, READ);
		mac[1] = twi_read();
		mac[0] = twi_read(); 
		twi_read(); twi_read();	//We don't care about the other two bytes
		*control = STOP|TWSIEN; 	//Send stop signal
		if(verbose) fprintf(stderr,"SENT Stop signal\n");

		printf("HWaddr %02x:%02x:%02x:%02x:%02x:%02x\n", mac[5],mac[4],mac[3],mac[2],mac[1],mac[0]);
	}

	if(display_mem || display_otp) {

		unsigned char dat[4];
		unsigned char start=addr;

		twi_select(AVR_ADDR, WRITE);

		for(;addr<(start+64);addr+=4) {

			twi_select(AVR_ADDR, WRITE);
			twi_write(OTP_R);
			twi_write(addr);
			twi_select(AVR_ADDR, READ);
			dat[0] = twi_read();
			dat[1] = twi_read();
			dat[2] = twi_read();
			dat[3] = twi_read();	
			*control = STOP|TWSIEN; 	//Send stop signal

			if(verbose) fprintf(stderr,"Sent Stop signal\n");
			fwrite(&dat[0], sizeof(unsigned int), 1, stdout);
			fflush(stdout);
		}
	}


	if(!did_something) usage(argv);

	return 0;
}
