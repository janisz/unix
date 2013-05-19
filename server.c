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

typedef struct  {
    int x;
    int y;
} Position;

typedef struct  {
    char nick[NICK_LENGTH];
    int attribute;
    Position position;
    int descriptor;
} Player;

typedef struct {
	int width;
	int height;
	char *map;
} Map;

Map createMap(int width, int height, char *map) 
{
	Map m;
	m.width = width;
	m.height = height;
	m.map = (char*)malloc(sizeof(char)*MAP_SIZE(m));
	if (map == 0) ERR("Cannot alocate memory for map");
	strncpy(m.map, map, MAP_SIZE(m));
	return m;
}

void deleteMap(Map *map)
{
	map->width = map->height = 0;
	free(map->map);	
}

Map readMapFromFile(char *filename) {
	int width, height;	
	
	FILE *fp;
	fp = fopen(filename, "r+");
	if (fp == NULL) ERR("Cannot open map file\n");
	
	fscanf(fp, "%d %d", &width, &height);
	char map[width*height];
	for (int i=0;i<width*height;i++) {
		fscanf(fp, "%c", &map[i]);
		if (isspace(map[i])) i--;
	}
	
	fclose(fp);
	
	return createMap(width, height, map); 
}

int indexOnMap(Map map, int x, int y) 
{
	return map.width*y+x;
}

void printMap(Map map)
{
	if (MAP_SIZE(map) < 1) {
		printf("Map is not loaded\n");
		return;
	}
	printf("%d %d\n", map.width, map.height);
	for (int j=0;j<map.height;j++) {
		for (int i=0;i<map.width;i++) {
			printf("%c", map.map[indexOnMap(map, i, j)]);
		}
		printf("\n");
	}
}

int main(int argc, char** argv)
{
	Map m = readMapFromFile("sample.map");
	printMap(m);
	deleteMap(&m);
	printMap(m);
    return EXIT_SUCCESS;
}

