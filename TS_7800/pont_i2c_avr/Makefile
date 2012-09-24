CC=avr-gcc
OBJCOPY=avr-objcopy
CFLAG=-g -mmcu=atmega88 -Wall -Wstrict-prototypes -Os -mcall-prologues -DF_CPU=1000000



all:	main.hex

main.hex:	main.out
	$(OBJCOPY) -R .eeprom -O ihex main.out main.hex

main.out:	main.o i2ctools.o serialtools.o
	$(CC) $(CFLAG) -o main.out -Wl,-Map,main.map main.o i2ctools.o serialtools.o

main.o:  main.c i2ctools.h
	$(CC) $(CFLAG) -c main.c

i2ctools.o:  i2ctools.c i2ctools.h
	$(CC) $(CFLAG) -c i2ctools.c

serialtools.o:	serialtools.c serialtools.h
	$(CC) $(CFLAG) -c serialtools.c

clean:
	rm -f *.o *.map *.out

flash:
	avrdude -c stk500 -P /dev/ttyUSB0 -p m88 -U flash:w:main.hex


verif:
	avrdude -c stk500 -P /dev/ttyUSB0 -p m88 -U flash:v:main.hex
