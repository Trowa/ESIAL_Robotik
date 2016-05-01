
.PHONY=srf04

CC=gcc
OBJS=main.o srf04.o

srf04: $(OBJS)
	$(CC) -Wall -O2 -o $@ $^

%.o: %.c
	$(CC) -Wall -O2 -c $^
