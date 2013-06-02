#include "gamelogic.h"
typedef int (*fptr)(Player*, char*);

int show(Player *player, char*)
{
	fprintf(stderr,"%s\t%s: Show request\n", __LINE__, player->nick);
}

int up(Player *player, char*)
{
	fprintf(stderr,"%s\t%s: UP request\n", __LINE__, player->nick);
}

int down(Player *player, char*)
{
	fprintf(stderr,"%s\t%s: DOWN request\n", __LINE__, player->nick);
}

int left(Player *player, char*)
{
	fprintf(stderr,"%s\t%s: LEFT request\n", __LINE__, player->nick);
}

int right(Player *player, char*)
{
	fprintf(stderr,"%s\t%s: RIGHT request\n", __LINE__, player->nick);
}

int attack(Player *player, char* opponentNick)
{
	fprintf(stderr,"%s\t%s: Attack %s request\n", __LINE__, player->nick, opponentNick);
}

int undefiniedAction(Player *player, char* action)
{
	fprintf(stderr,"%s\t%s: Undefinied request: %s\n", __LINE__, player->nick, action);
}

fptr actionFactory(char action[MSG_LENGTH], char *ret)
{
	ret = NULL;
	if (strcmp(action, SHOW) == 0) {
		return show;
	} else if (strcmp(action, UP) == 0) {
		return up;
	} else if (strcmp(action, DOWN) == 0) {
		return down;
	} else if (strcmp(action, LEFT) == 0) {
		return left;
	} else if (strcmp(action, RIGHT) == 0) {
		return right;
	} else if (strstr(action, ATTACK) == action) {
		ret = action + strlen(ATTACK);
		return attack;
	}

	ret = action;
	return undefiniedAction;
}
