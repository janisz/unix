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
#include "map.h"

#define ERR(source) (fprintf(stderr,"%s:%d\n",__FILE__,__LINE__),\
                     perror(source),kill(0,SIGKILL),\
		     		     exit(EXIT_FAILURE))

void usage(char *name)
{
	fprintf(stderr,"USAGE: %s ip port\n",name);
}

void parse_arguments(int argc, char** argv,	struct in_addr *addr, int *port )
{
	if(argc != 3) {
		usage(argv[0]);
		exit(EXIT_FAILURE);
	}

	if ((*port = atoi(argv[2])) < 0) {
		usage(argv[0]);
		exit(EXIT_FAILURE);
	}

	if (inet_aton(argv[1], addr) == 0) {
		usage(argv[0]);
		exit(EXIT_FAILURE);
	}
}

int main(int argc, char** argv)
{
	int socket, port;
	struct in_addr ifaddr;

	parse_arguments(argc, argv, &ifaddr, &port);

	socket = connect_inet_socket(ifaddr, port, SOCK_STREAM);

	char nick[NICK_LENGTH];
	printf("Enter nick: ");
	scanf("%s", nick);

	if(bulk_write(socket, nick, NICK_LENGTH) < NICK_LENGTH) {
		fprintf(stderr,"server did not recive nick");
	}

	int width, height;
	if (bulk_read(socket, &width, sizeof(width)) < 0) {
		fprintf(stderr,"width read problem");
	}
	if (bulk_read(socket, &height, sizeof(height)) < 0) {
		fprintf(stderr,"height read problem");
	}
	char rawMap[width*height];
	if (bulk_read(socket, &rawMap, width*height) < 0) {
		fprintf(stderr,"Map read problem");
	}

	Map m = createMap(width, height, rawMap);
	printMap(m);

	if((TEMP_FAILURE_RETRY(close(socket)))<0) {
		ERR("close");
	}

	return EXIT_SUCCESS;
}
