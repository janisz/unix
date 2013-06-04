#include "gamelogic.h"

void broadcast(Player *player, char *msg)
{
	DBG;
	fprintf(stderr,"%s: Broadcast\n", player->nick);
	for (int i=0; i<arraylist_size(player->players); i++) {
		Player* p = arraylist_get(player->players, i);
		pthread_mutex_lock(p->bufforLock);
		arraylist_add(p->buffor, msg);
		pthread_mutex_unlock(p->bufforLock);
		pthread_cond_signal(p->bufforCondition);
	}
}

void broadcastWithNick(Player *player, char *msg)
{
	char *m = (char*)malloc(MSG_LENGTH);
	snprintf(m, MSG_LENGTH, "%s %s", player->nick, msg);
	broadcast(player, m);
}

int show(Player *player, char* nil)
{
	DBG;
	fprintf(stderr,"%s: Show request\n", player->nick);

	arraylist *list = arraylist_create();

	for (int i=0; i<arraylist_size(player->players); i++) {
		Player* p = arraylist_get(player->players, i);
		char *msg = (char*)malloc(MSG_LENGTH);
		snprintf(msg, MSG_LENGTH, "%s position: %d", p->nick, p->position);
		arraylist_add(list, msg);
	}

	pthread_mutex_lock(player->bufforLock);
	arraylist_join(player->buffor, list);
	pthread_mutex_unlock(player->bufforLock);
	pthread_cond_signal(player->bufforCondition);

	arraylist_destroy(list);

	return 0;
}

int join(Player *player, char *nil)
{
	DBG;
	fprintf(stderr,"%s: Join Game\n", player->nick);

	char *msg = (char*)malloc(MSG_LENGTH);
	snprintf(msg, MSG_LENGTH, "%s join game", player->nick);

	broadcast(player, msg);

	return 0;
}

int leftGame(Player *player, char *nil)
{
	DBG;
	fprintf(stderr,"%s: Left Game\n", player->nick);

	char *msg = (char*)malloc(MSG_LENGTH);
	snprintf(msg, MSG_LENGTH, "%s left game", player->nick);

	broadcast(player, msg);

	return 0;
}

int up(Player *player, char* nil)
{
	DBG;
	fprintf(stderr,"%s: UP request\n", player->nick);
	broadcastWithNick(player, "Go up");
	return 0;
}

int down(Player *player, char* nil)
{
	DBG;
	fprintf(stderr,"%s: DOWN request\n", player->nick);
	broadcastWithNick(player, "Go down");
	return 0;
}

int left(Player *player, char* nil)
{
	DBG;
	fprintf(stderr,"%s: LEFT request\n", player->nick);
	broadcastWithNick(player, "Go left");
	return 0;
}

int right(Player *player, char* nil)
{
	DBG;
	fprintf(stderr,"%s: RIGHT request\n", player->nick);
	broadcastWithNick(player, "Go right");
	return 0;
}

int attack(Player *player, char* opponentNick)
{
	DBG;
	fprintf(stderr,"%s: Attack %s request\n", player->nick, opponentNick);
	broadcastWithNick(player, "Attack");
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
