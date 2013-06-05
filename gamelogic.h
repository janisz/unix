#ifndef _GAME_LOGIC_H_

#include "constans.h"
#include "player.h"
#include <string.h>

typedef int (*fptr)(Player*, char*);

int show(Player *player, char*);

int up(Player *player, char*);

int down(Player *player, char*);

int left(Player *player, char*);

int right(Player *player, char*);

int attack(Player *player, char* opponentNick);

int undefinied(Player *player, char* action);

fptr actionFactory(char action[MSG_LENGTH], char** ret);

int join(Player *player, char*);

int leftGame(Player *player, char*);

#define _GAME_LOGIC_H_
#endif
