#include "player.h"

#include <errno.h>
#include <unistd.h>
#include <assert.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>


Player* createPlayer(const char *nick, int attribute, int position,
					 int descriptor, arraylist *players)
{
	Player *p = (Player*)malloc(sizeof(Player));
	assert(p);
	strncpy(p->nick, nick, NICK_LENGTH);
	p->attribute = attribute;
	p->position = position;
	p->descriptor = descriptor;
	p->players = players;
	return p;
}

void disposePlayer(Player *player)
{
	if(TEMP_FAILURE_RETRY(close(player->descriptor))<0)ERR("close");
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
