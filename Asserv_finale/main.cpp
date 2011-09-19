#include "mbed.h"

DigitalOut myled1(LED1);
DigitalOut myled2(LED2);
DigitalOut myled3(LED3);
DigitalOut myled4(LED4);

int main() {
    while(1) {
        myled1 = 1;
        myled2 = 0;
        wait(0.05);
        myled1 = 0;
        myled2 = 1;
        wait(0.05);
        myled2 = 0;
        myled3 = 1;
        wait(0.05);
        myled3 = 0;
        myled4 = 1;
        wait(0.05);
        myled4 = 0;
        myled3 = 1;
        wait(0.05);
        myled3 = 0;
        myled2 = 1;
        wait(0.05);
    }
}
