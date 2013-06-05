#ifndef _MAP_H_

#define _GNU_SOURCE
#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#include "constans.h"
#include "datastructs/src/arraylist.h"

#define ERR(source) (fprintf(stderr,"%s:%d\n",__FILE__,__LINE__),\
                     perror(source),kill(0,SIGKILL),\
		     		     exit(EXIT_FAILURE))

typedef struct {
	int width;
	int height;
	char *map;
	pthread_mutex_t *mutexs;
	arraylist *rooms;
} Map;

Map createMap(int width, int height, char *map);
void deleteMap(Map *map);
Map readMapFromFile(char *filename);
int indexOnMap(Map map, int x, int y);
int getMapSize(Map *map);
int getCartesian(Map *map, int index, int *x, int *y);
char getField(int x, int y);
int getRandomRoom(Map *m);
void printMap(Map map);
void printRooms(Map *map);

#define _MAP_H_
#endif
