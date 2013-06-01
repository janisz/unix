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
#include <sys/types.h>
#include <pthread.h>
#include <sys/types.h>
#include <signal.h>

#include "comunication.h"
#include "constans.h"
#include "map.h"
#include "player.h"



int sethandler( void (*f)(int), int sigNo)
{
	struct sigaction act;
	memset(&act, 0, sizeof(struct sigaction));
	act.sa_handler = f;
	if (-1==sigaction(sigNo, &act, NULL))
		return -1;
	return 0;
}

void usage(char *name)
{
	printf("USAGE: %s port\n", name);
}

void* clientReader(void* data)
{
	int fd = *((int*)data);

	while (TRUE) {
		char msg[MSG_LENGTH] = {0};
		if (bulk_read(fd, &msg, MSG_LENGTH) < MSG_LENGTH) {
			fprintf(stderr,"client read problem\n");
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

	while (TRUE) {
		char msg[MSG_LENGTH] = {0};
		sleep(5);
		i++;
		sprintf(msg, "#%d message", i);
		if(bulk_write(fd, msg, MSG_LENGTH) < MSG_LENGTH) {
			fprintf(stderr,"player did not recive message\n");
			pthread_exit(NULL);
		}
		fprintf(stderr,"Sent: %s\n", msg);
	}

	return 0;
}

Player* initializePlayer(int socket, char nick[NICK_LENGTH], Map *map)
{
	Player *player;
	int att = rand() % MAX_ATTRIBUTE;
	int pos = getRandomRoom(map);

	fprintf(stderr,"Create new player\n");
	player = createPlayer(nick, att, pos, socket);
	showPlayerInfo(player);

	pthread_create(&player->reader,NULL,clientReader,&socket);
	pthread_detach(player->reader);
	pthread_create(&player->writer,NULL,clientWriter,&socket);
	pthread_detach(player->writer);

	return player;
}

int isNickValid(arraylist *players, char nick[NICK_LENGTH])
{
	fprintf(stderr,"Check if %s is already used\nPlayers count: %d\n", nick, arraylist_size(players));	
	for (int i=0;i<arraylist_size(players); i++) {		
		Player *p = (Player*)arraylist_get(players, i);		
		if (strcmp(p->nick, nick) == 0) {			
			return 0;
		}
	}
	return 1;
}

void addNewPlayer(int socket, arraylist *players, Map *map)
{
	fprintf(stderr,"Player #%d name: ", arraylist_size(players));

	char buf[MSG_LENGTH] = {0};
	strncpy(buf, "Nick: ", MSG_LENGTH);
	if (bulk_write(socket, buf, MSG_LENGTH) < 0) {
		fprintf(stderr,"player write problem");			
	}

	char nick[NICK_LENGTH];
	if (bulk_read(socket, &nick, NICK_LENGTH) < 0) {
		fprintf(stderr,"nick read problem");
		return;
	}

	fprintf(stderr,"%s\n", nick);
	if (isNickValid(players, nick)) {
		arraylist_add(players, initializePlayer(socket, nick, map));
	} else {
		
		strncpy(buf, "Wrong nick\n", MSG_LENGTH);
		fprintf(stderr,"%s", buf);	
		if (bulk_write(socket, buf, MSG_LENGTH) < 0) {
			fprintf(stderr,"player write problem");			
		}
		if(TEMP_FAILURE_RETRY(close(socket))<0)ERR("close");
	}
}

void waitForPlayers(int sfd, uint32_t port, Map map)
{
	int nfd;
	struct sockaddr_in client;
	socklen_t clen;
	arraylist *players = arraylist_create();

	fprintf(stderr,"Waiting for clients:\n");

	while(TRUE) {

		if((nfd=accept(sfd, (struct sockaddr *)&client, &clen))<0) {
			if(EINTR==errno) continue;
			ERR("accept");
		}

		addNewPlayer(nfd, players, &map);

	}
}

int main(int argc, char** argv)
{
	if(argc != 2) {
		usage(argv[0]);
		return EXIT_FAILURE;
	}

	signal(SIGPIPE, SIG_IGN);

	Map m = readMapFromFile("sample.map");
	printMap(m);

	int socket, port = atoi(argv[1]);
	socket=bind_inet_socket(port, SOCK_STREAM);

	waitForPlayers(socket, port, m);

	if(TEMP_FAILURE_RETRY(close(socket))<0)ERR("close");

	deleteMap(&m);
	printMap(m);

	return EXIT_SUCCESS;
}

