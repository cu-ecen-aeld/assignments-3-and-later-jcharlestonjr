CROSS_COMPILE ?=

CC=$(CROSS_COMPILE)gcc

all: aesdsocket
default: aesdsocket

aesdsocket:aesdsocket.c
	$(CC) -g -Wall -o aesdsocket aesdsocket.c

clean:
	$(RM) aesdsocket

