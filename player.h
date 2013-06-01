#ifndef _PLAYER_H_

#include "constans.h"

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

Player createPlayer(const char *nick, int attribute, Position position, int descriptor);
void disposePlayer(Player player);


#define _PLAYER_H_
#endif
