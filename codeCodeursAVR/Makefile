CC=avr-gcc
FILE=main
OBJCOPY=avr-objcopy
CFLAG=-g -mmcu=atmega88 -Wall -Wstrict-prototypes -Os -mcall-prologues -DF_CPU=1000000



all:	$(FILE).hex

$(FILE).hex:	$(FILE).out
	$(OBJCOPY) -R .eeprom -O ihex $(FILE).out $(FILE).hex

$(FILE).out:	$(FILE).o
	$(CC) $(CFLAG) -o $(FILE).out -Wl,-Map,$(FILE).map $(FILE).o

$(FILE).o:  $(FILE).c
	$(CC) $(CFLAG) -c $(FILE).c

clean:	
	rm -f *.o *.map *.out

flash:
	avrdude -c stk500 -P /dev/ttyUSB0 -p m88 -U flash:w:$(FILE).hex
	

verif:
	avrdude -c stk500 -P /dev/ttyUSB0 -p m88 -U flash:v:$(FILE).hex
