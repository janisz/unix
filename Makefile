CFLAGS := -Wall -pedantic -std=gnu99 
CC := color-gcc

all: server client

server: server.o comunication.o map.o arraylist.o player.o
	$(CC) server.o comunication.o map.o arraylist.o player.o -o server -lpthread

server.o: server.c constans.h map.h
	$(CC) -c server.c $(CFLAGS) 

client: client.o comunication.o map.o arraylist.o
	$(CC) client.o comunication.o map.o arraylist.o -o client -lpthread

client.o: client.c
	$(CC) -c client.c $(CFLAGS) 

comunication.o: comunication.c comunication.h
	$(CC) -c comunication.c $(CFLAGS) 

map.o: map.c map.h 
	$(CC) -c map.c $(CFLAGS) 

player.o: player.c player.h 
	$(CC) -c player.c $(CFLAGS) 

arraylist.o: datastructs/src/arraylist.h datastructs/src/arraylist.c
	$(CC) -c datastructs/src/arraylist.c $(CFLAGS)

zip: style all clean
	tar -jcvf ${USER}.tar.bz2 *	

style:
	astyle -A8 -T *.c

clean:
	rm -rf *.o server client *.orig ${USER}.tar.bz
