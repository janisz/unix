/* This is a Cfunctions (version 0.28) generated header file.
   Cfunctions is a free program for extracting headers from C files.
   Get Cfunctions from 'http://www.lemoda.net/cfunctions/'. */

/* This file was generated with:
'/home/protected/bin/cfunctions -f server.c -w server.c -n -c' */
#ifndef CFH_SERVER_C
#define CFH_SERVER_C

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


#include "datastructs/src/arraylist.h"
#include "comunication.h"
#include "constans.h"
#include "map.h"
#include "player.h"


#line 28 "server.c"
int sethandler (void ( *f ) ( int ) , int sigNo );

#line 38 "server.c"
void usage (char *name );

#line 43 "server.c"
int findPlayerWithNick (arraylist *players , char nick[NICK_LENGTH] );

#line 55 "server.c"
void removePlayer (Player *player );

#line 66 "server.c"
void * clientReader (void * data );

#line 84 "server.c"
void * clientWriter (void * data );

#line 104 "server.c"
Player * initializePlayer (int socket , char nick[NICK_LENGTH] , Map *map , arraylist *players );

#line 122 "server.c"
int isNickValid (arraylist *players , char nick[NICK_LENGTH] );

#line 128 "server.c"
void addNewPlayer (int socket , arraylist *players , Map *map );

#line 158 "server.c"
void waitForPlayers (int sfd , uint32_t port , Map map );

#endif /* CFH_SERVER_C */
