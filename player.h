#ifndef _PLAYER_H_
#define _GNU_SOURCE

#include <pthread.h>
#include "constans.h"
#include "map.h"

#include "datastructs/src/arraylist.h"

typedef struct  {
	char nick[NICK_LENGTH];
	int attribute;
	int position;
	int descriptor;
	pthread_t reader;
	pthread_t writer;
	arraylist *players;
	arraylist* buffor;
	pthread_mutex_t bufforLock[1];
	pthread_cond_t  bufforCondition[1];
	Map* map;
} Player;

Player* createPlayer(const char *nick, int attribute, int position, int descriptor, arraylist *players, Map* map);
void disposePlayer(Player *player);
int findPlayerIndexWithNick (arraylist *players , char nick[NICK_LENGTH] );
Player* findPlayerWithNick (arraylist *players , char nick[NICK_LENGTH] );
void removePlayer(Player *player);
void disconnectPlayer(Player *player);
void showPlayerInfo(Player *p);

#define _PLAYER_H_
#endif
