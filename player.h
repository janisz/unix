#ifndef _PLAYER_H_

#include <pthread.h>
#include "constans.h"

typedef struct  {
	char nick[NICK_LENGTH];
	int attribute;
	int position;
	int descriptor;
	pthread_t reader;
	pthread_t writer;
} Player;

Player createPlayer(const char *nick, int attribute, int position, int descriptor);
void disposePlayer(Player player);


#define _PLAYER_H_
#endif
