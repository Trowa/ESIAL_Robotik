#include "ts7800io.h"
#include "memoryAccess.h"
#include "twsi.h"
#include <signal.h>

void initIO() {
	memoryInit();
	signal(SIGTERM, exit_gracefully);
	signal(SIGHUP, exit_gracefully);
	signal(SIGINT, exit_gracefully);
	signal(SIGPIPE, exit_gracefully);
}

void inline setGreenLedOn() {
	setBitRegister(8, 30, 1);
}
void inline setGreenLedOff() {
	setBitRegister(8, 30, 0);
}

void setRedLedOn() {
	twi_select(AVR_ADDR, WRITE);
	twi_write(LED_ON);
	twi_stop();
}
void setRedLedOff() {
	twi_select(AVR_ADDR, WRITE);
	twi_write(LED_ON);
	twi_stop();
}

uint8_t getGreenLedStatus() {
	return getBitRegister(8, 30);
}
