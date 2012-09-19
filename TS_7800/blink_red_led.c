#include <stdio.h>
#include "memoryAccess.h"


int main() {
	    
    while(1) {
		twi_select(AVR_ADDR, WRITE);
		twi_write(LED_ON);
		*control = STOP|TWSIEN; 	//Send stop signal
		usleep(100000);
		twi_select(AVR_ADDR, WRITE);
		twi_write(LED_OFF);
		*control = STOP|TWSIEN; 	//Send stop signal
		sleep(1);
	}
	
	return 0;
	
}
