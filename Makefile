CFLAGS := -Wall -pedantic -std=gnu99 

all: signals_1a

signals_1a: signals_1a.o 
	gcc signals_1a.o -o signals_1a -lpthread

signals_1a.o: signals_1a.c jlib.h
	gcc -c signals_1a.c $(CFLAGS) 

zip: signals_1a clean
	tar -jcvf ${USER}.tar.bz2 *

style:
	astyle -A8 *.c

clean:
	rm -rf *o signals_1a ${USER}.tar.bz
