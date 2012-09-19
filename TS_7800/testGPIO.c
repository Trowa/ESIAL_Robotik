#include "ts7800io.h"
#include <stdio.h>
#include <unistd.h>

int main() {
	
	initIO();
	
	gpio_t gpioA26 = getGPIO('A', 26);
	setGPIOInput(gpioA26);
	
	while(1) {
		setGPIOValue(gpioA26, HIGH);
		printf("gpioA26 HIGH - valeur lue : %d\n", getGPIOValue(gpioA26));
		
		usleep(300000);
		setGPIOValue(gpioA26, LOW);
		printf("gpioA26 LOW - valeur lue : %d\n", getGPIOValue(gpioA26));
		usleep(300000);
	}
}
