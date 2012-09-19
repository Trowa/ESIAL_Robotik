testGPIO : ts7800io.o ts7800_gpio.o memoryAccess.o testGPIO.o
	gcc -g -Wall -o testGPIO ts7800io.o ts7800_gpio.o memoryAccess.o testGPIO.o
	
testGPIO.o: testGPIO.c ts7800io.h ts7800_gpio.h
	gcc -g -Wall -c testGPIO.c

blink_green_led: blink_green_led.o memoryAccess.o ts7800io.o
	gcc -g -Wall -o blink_green_led blink_green_led.o memoryAccess.o ts7800io.o
	
blink_green_led.o:	blink_green_led.c ts7800io.h
	gcc -g -Wall -c blink_green_led.c

ts7800io.o:	ts7800io.c ts7800io.h memoryAccess.h
	gcc -g -Wall -c ts7800io.c

ts7800_gpio.o: ts7800_gpio.c ts7800_gpio.h memoryAccess.h
	gcc -g -Wall -c ts7800_gpio.c
	
memoryAccess.o:	memoryAccess.c memoryAccess.h
	gcc -g -Wall -c memoryAccess.c

clean:
	rm *.o testGPIO blink_green_led
