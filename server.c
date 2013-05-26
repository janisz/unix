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
#include <sys/types.h>
#include <pthread.h>

#include "comunication.h"
#include "constans.h"
#include "map.h"


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


void usage(char *name)
{
	printf("USAGE: %s port\n", name);
}

void* clientReader(void* data)
{
	int fd = *((int*)data);

	while (1) {
		char msg[MSG_LENGTH] = {0};
		if (bulk_read(fd, &msg, MSG_LENGTH) < MSG_LENGTH) {
			fprintf(stderr,"client read problem");
			pthread_exit(NULL);
		}
		fprintf(stderr,"Recive: %s\n", msg);
	}

	return 0;
}

void* clientWriter(void* data)
{
	int fd = *((int*)data);
	int i=0;

	while (1) {
		char msg[MSG_LENGTH] = {0};
		sleep(5);
		i++;
		sprintf(msg, "#%d message", i);
		if(bulk_write(fd, msg, MSG_LENGTH) < MSG_LENGTH) {
			fprintf(stderr,"player did not recive map");
			pthread_exit(NULL);
		}
		fprintf(stderr,"Sent: %s\n", msg);
	}

	return 0;
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

		fprintf(stderr,"Player #%d name: ", i);

		char nick[NICK_LENGTH];

		if (bulk_read(nfd, &nick, NICK_LENGTH) < 0) {
			fprintf(stderr,"nick read problem");
		}

		fprintf(stderr,"%s\n", nick);

		if(bulk_write(nfd, &map.width, sizeof(map.width)) < sizeof(map.width)) {
			fprintf(stderr,"player did not recive map width");
		}

		if(bulk_write(nfd, &map.height, sizeof(map.height)) < sizeof(map.height)) {
			fprintf(stderr,"player did not recive map height");
		}

		if(bulk_write(nfd, map.map, MAP_SIZE(map)) < MAP_SIZE(map)) {
			fprintf(stderr,"player did not recive map");
		}

		pthread_t reader, writer;
		pthread_create(&reader,NULL,clientReader,&nfd);
		pthread_detach(reader);
		pthread_create(&writer,NULL,clientWriter,&nfd);
		pthread_detach(writer);
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

