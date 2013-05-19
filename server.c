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

#include "comunication.h"
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

/*--------------------------------------------------------------------*/

Player createPlayer(const char *nick, int attribute, Position position, int descriptor)
{
	Player p;
	strncpy(p.nick, nick, NICK_LENGTH);
	p.attribute = attribute;
	p.position = position;
	descriptor = descriptor;
	return p;
}

void disposePlayer(Player player)
{
	close(player.descriptor);
}

/*--------------------------------------------------------------------*/

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

Map readMapFromFile(char *filename)
{
	int width, height;

	FILE *fp;
	fp = fopen(filename, "r+");
	if (fp == NULL) ERR("Cannot open map file\n");

	fscanf(fp, "%d %d", &width, &height);
	char map[width*height];
	for (int i=0; i<width*height; i++) {
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
	for (int j=0; j<map.height; j++) {
		for (int i=0; i<map.width; i++) {
			printf("%c", map.map[indexOnMap(map, i, j)]);
		}
		printf("\n");
	}
}

/*--------------------------------------------------------------------*/

void usage(char *name)
{
	printf("USAGE: %s port\n", name);
}

void addNewClients(int sfd, uint32_t port, Map map)
{
	int nfd, i=0;
	struct sockaddr_in client;
	socklen_t clen;

	fprintf(stderr,"Waiting for clients:\n");

	while(TRUE) {
		i++;
		if((nfd=accept(sfd, (struct sockaddr *)&client, &clen))<0) {
			if(EINTR==errno) continue;
			ERR("accept");
		}
		
		printf("Player #%d name: ", i);
		
		char nick[NICK_LENGTH];
		
		if (bulk_read(nfd, &nick, NICK_LENGTH) < 0) {
			fprintf(stderr,"nick read problem");		
		}
		
		printf("%s", nick);
		
		if(bulk_write(nfd, &map.width, sizeof(map.width)) < sizeof(map.width)) {
			fprintf(stderr,"player did not recive map width");		
		}
		
		if(bulk_write(nfd, &map.height, sizeof(map.height)) < sizeof(map.height)) {
			fprintf(stderr,"player did not recive map height");		
		}

		if(bulk_write(nfd, map.map, MAP_SIZE(map)) < MAP_SIZE(map)) {
			fprintf(stderr,"player did not recive map");		
		}
		
		close(nfd);
	}
}

int main(int argc, char** argv)
{
	if(argc != 2) {
		usage(argv[0]);
		return EXIT_FAILURE;
	}
	
	Map m = readMapFromFile("sample.map");
	printMap(m);
	
	int socket, port = atoi(argv[1]);
	socket=bind_inet_socket(port, SOCK_STREAM);
	
	addNewClients(socket, port, m);
	
	if(TEMP_FAILURE_RETRY(close(socket))<0)ERR("close");	
	
	deleteMap(&m);
	printMap(m);
	
	return EXIT_SUCCESS;
}

