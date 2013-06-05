#include "player.h"

#include <errno.h>
#include <unistd.h>
#include <assert.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

Player* createPlayer(const char *nick, int attribute, int position,
					 int descriptor, arraylist *players, Map* map)
{
	Player *p = (Player*)malloc(sizeof(Player));
	assert(p);
	strncpy(p->nick, nick, NICK_LENGTH);
	p->attribute = attribute;
	p->position = position;
	p->descriptor = descriptor;
	p->players = players;
	p->buffor = arraylist_create();
	p->map = map;
	pthread_mutex_init((p->bufforLock), NULL);
	pthread_cond_init((p->bufforCondition), NULL);
	return p;
}

void disposePlayer(Player *p)
{
	disconnectPlayer(p);
	pthread_mutex_lock(p->bufforLock);
	arraylist_destroy(p->buffor);
	pthread_mutex_unlock(p->bufforLock);
	pthread_cond_destroy((p->bufforCondition));
	pthread_mutex_destroy((p->bufforLock));
}

void showPlayerInfo(Player *p)
{
	char buf[INET_ADDRSTRLEN] = "";
	struct sockaddr_in name;
	socklen_t len = sizeof(name);

	if (getpeername(p->descriptor, (struct sockaddr *)&name, &len) != 0) {
		perror("getpeername");
	} else {
		inet_ntop(AF_INET, &name.sin_addr, buf, sizeof buf);
	}
	printf("Nick: %s \t Att: %d \nPosition: %d, IP: %s\n",
		   p->nick, p->attribute, p->position, buf);
}

int findPlayerIndexWithNick(arraylist *players, char nick[NICK_LENGTH])
{
	fprintf(stderr,"Search for %s\nPlayers count: %d\n", nick, arraylist_size(players));
	for (int i=0; i<arraylist_size(players); i++) {
		Player *p = (Player*)arraylist_get(players, i);
		if (strcmp(p->nick, nick) == 0) {
			return i;
		}
	}
	return -1;
}

int findPlayerIndex(Player *player)
{
	DBG;
	fprintf(stderr,"Search for %s\nPlayers count: %d\n", player->nick, arraylist_size(player->players));
	for (int i=0; i<arraylist_size(player->players); i++) {
		Player *p = (Player*)arraylist_get(player->players, i);
		if (strcmp(p->nick, player->nick) == 0) {
			return i;
		}
	}
	return -1;
}

Player* findPlayerWithNick(arraylist *players, char nick[NICK_LENGTH])
{
	int index = findPlayerIndexWithNick(players, nick);
	if (index < 0) {
		DBG;
		fprintf(stderr,"Not found %s\n", nick);
		return NULL;
	}
	return arraylist_get(players, index);
}

int fd_is_valid(int fd)
{
	return fcntl(fd, F_GETFD) != -1 || errno != EBADF;
}

void disconnectPlayer(Player *player)
{
	if (fd_is_valid(player->descriptor))
		if(TEMP_FAILURE_RETRY(close(player->descriptor))<0)ERR("close");
}

void removePlayer(Player *player)
{
	pthread_mutex_lock(player->players->lock);
	fprintf(stderr,"%s will be removed\nPlayers count: %d\n", player->nick, arraylist_size(player->players));
	int index = findPlayerIndex(player);
	assert(index >= 0);
	disposePlayer(player);
	arraylist_remove(player->players, index);
	fprintf(stderr,"Players count: %d\n", arraylist_size(player->players));
	pthread_mutex_unlock(player->players->lock);
}
