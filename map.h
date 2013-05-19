#ifndef _MAP_H_

#define _GNU_SOURCE
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

#include "constans.h"

#define ERR(source) (fprintf(stderr,"%s:%d\n",__FILE__,__LINE__),\
                     perror(source),kill(0,SIGKILL),\
		     		     exit(EXIT_FAILURE))

typedef struct {
	int width;
	int height;
	char *map;
} Map;

Map createMap(int width, int height, char *map);
void deleteMap(Map *map);
Map readMapFromFile(char *filename);
int indexOnMap(Map map, int x, int y);
void printMap(Map map);


#define _MAP_H_
#endif
