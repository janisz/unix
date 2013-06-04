#ifndef _PLAYER_H_
#define _GNU_SOURCE

#include <pthread.h>
#include "constans.h"

#include "datastructs/src/arraylist.h"

typedef struct  {
	char nick[NICK_LENGTH];
	int attribute;
	int position;
	int descriptor;
	pthread_t reader;
	pthread_t writer;
	arraylist *players;
	char* buffor;
	pthread_mutex_t bufforLock[1];
	pthread_cond_t  bufforCondition[1];
} Player;

Player* createPlayer(const char *nick, int attribute, int position, int descriptor, arraylist *players);
void disposePlayer(Player *player);


#define _PLAYER_H_
#endif
