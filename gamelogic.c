#include "gamelogic.h"

int show(Player *player, char* nil)
{
	DBG;
	fprintf(stderr,"%s: Show request\n", player->nick);
	return 0;
}

int up(Player *player, char* nil)
{
	DBG;
	fprintf(stderr,"%s: UP request\n", player->nick);
	return 0;
}

int down(Player *player, char* nil)
{
	DBG;
	fprintf(stderr,"%s: DOWN request\n", player->nick);
	return 0;
}

int left(Player *player, char* nil)
{
	DBG;
	fprintf(stderr,"%s: LEFT request\n", player->nick);
	return 0;
}

int right(Player *player, char* nil)
{
	DBG;
	fprintf(stderr,"%s: RIGHT request\n", player->nick);
	return 0;
}

int attack(Player *player, char* opponentNick)
{
	DBG;
	fprintf(stderr,"%s: Attack %s request\n", player->nick, opponentNick);
	return 0;
}

int undefiniedAction(Player *player, char* action)
{
	DBG;
	fprintf(stderr,"%s: Undefinied request: %s\n", player->nick, action);
	return 0;
}

fptr actionFactory(char action[MSG_LENGTH], char** ret)
{
	*ret = NULL;
	if  (strstr(action, SHOW) == action) {
		return show;
	} else if  (strstr(action, UP) == action) {
		return up;
	} else if  (strstr(action, DOWN) == action) {
		return down;
	} else if  (strstr(action, LEFT) == action) {
		return left;
	} else if  (strstr(action, RIGHT) == action) {
		return right;
	} else if (strstr(action, ATTACK) == action) {
		*ret = action + strlen(ATTACK) + 1;
		return attack;
	}

	*ret = action;
	return undefiniedAction;
}
