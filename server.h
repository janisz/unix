#ifndef _SERVER_H_
#define _SERVER_H_

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

int sethandler (void ( *f ) ( int ) , int sigNo );
void usage (char *name );
int findPlayerIndexWithNick (arraylist *players , char nick[NICK_LENGTH] );
Player* findPlayerWithNick (arraylist *players , char nick[NICK_LENGTH] );
void removePlayer (Player *player );
void * clientReader (void * data );
void * clientWriter (void * data );
Player * initializePlayer (int socket , char nick[NICK_LENGTH] , Map *map , arraylist *players );int isNickValid (arraylist *players , char nick[NICK_LENGTH] );
void addNewPlayer (int socket , arraylist *players , Map *map );
void waitForPlayers (int sfd , uint32_t port , Map map );

#endif /* _SERVER_H_ */
