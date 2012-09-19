#include "ts7800io.h"
#include <stdio.h>
#include <unistd.h>

int main() {
	
	initIO();
	
	while(1) {
		setGreenLedOn();
		sleep(1);
		setGreenLedOff();
		sleep(1);
	}
	
}
