CFLAGS := -Wall -pedantic -std=gnu99 

all: server

server: server.o 
	gcc server.o -o server -lpthread

server.o: server.c jlib.h
	gcc -c server.c $(CFLAGS) 

zip: server clean
	tar -jcvf ${USER}.tar.bz2 *

style:
	astyle -A8 -T *.c

clean:
	rm -rf *o server ${USER}.tar.bz
