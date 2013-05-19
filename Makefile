CFLAGS := -Wall -pedantic -std=gnu99 

all: server client

server: server.o comunication.o map.o
	gcc server.o comunication.o map.o -o server -lpthread

server.o: server.c constans.h
	gcc -c server.c $(CFLAGS) 

client: client.o comunication.o map.o
	gcc client.o comunication.o map.o -o client -lpthread

client.o: client.c
	gcc -c client.c $(CFLAGS) 

comunication.o: comunication.c comunication.h
	gcc -c comunication.c $(CFLAGS) 

map.o: map.c map.h
	gcc -c map.c $(CFLAGS) 

zip: server clean
	tar -jcvf ${USER}.tar.bz2 *

style:
	astyle -A8 -T *.c

clean:
	rm -rf *o server ${USER}.tar.bz
