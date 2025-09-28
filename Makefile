CC = gcc
CFLAGS = -Wall -Wextra -O2
LDLIBS = -lpcap

all: tcp_counter

tcp_counter: main.c
	$(CC) $(CFLAGS) -o $@ main.c $(LDLIBS)

clean:
	rm -f tcp_counter

.PHONY: all clean
