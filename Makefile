CFLAGS := -Wall -pedantic -std=gnu99  -fgnu89-inline

all: server client

debug: CC += -DDEBUG -g
debug: server client

server: server.o comunication.o map.o arraylist.o player.o gamelogic.o
	$(CC) server.o comunication.o map.o arraylist.o player.o gamelogic.o -o server -lpthread

server.o: server.c constans.h map.h gamelogic.h
	$(CC) -c server.c $(CFLAGS) 

gamelogic.o: gamelogic.c gamelogic.h
	$(CC) -c gamelogic.c $(CFLAGS) 

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
