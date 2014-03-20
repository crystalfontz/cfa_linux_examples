#Makefile for Crystalfontz UNIX examples

CC = gcc
LIBS = 
CFLAGS = -g -O -Iinclude -Wall
LDFLAGS = -g

LIBSRC = include/serial.c include/cf_packet.c include/show_packet.c
LIBOBJ = $(LIBSRC:%.c=%.o)

all: example-632_634 example-635 example-631 example-633

example-632_634: 632_634/example-632_634.o $(LIBOBJ)
	$(CC) $(LDFLAGS) $(LIBOBJ) 632_634/example-632_634.o -o example-632_634

example-631: 631/example-631.o $(LIBOBJ)
	$(CC) $(LDFLAGS) $(LIBOBJ) 631/example-631.o -o example-631

example-633: 633/example-633.o $(LIBOBJ)
	$(CC) $(LDFLAGS) $(LIBOBJ) 633/example-633.o -o example-633

example-635: 635/example-635.o $(LIBOBJ)
	$(CC) $(LDFLAGS) $(LIBOBJ) 635/example-635.o -o example-635

clean:
	rm -f $(LIBOBJ) 632_634/example-632_634.o example-632_634 631/example-631.o example-631 633/example-633.o example-633 635/example-635.o example-635
