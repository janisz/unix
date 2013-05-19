CFLAGS := -Wall -pedantic -std=gnu99 

all: server

server: server.o comunication.o
	gcc server.o comunication.o -o server -lpthread

server.o: server.c constans.h
	gcc -c server.c $(CFLAGS) 

comunication.o: comunication.c comunication.h
	gcc -c comunication.c $(CFLAGS) 

zip: server clean
	tar -jcvf ${USER}.tar.bz2 *

style:
	astyle -A8 -T *.c

clean:
	rm -rf *o server ${USER}.tar.bz
