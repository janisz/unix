#include "gamelogic.h"

#include "map.h"

void sentFeedback(Player *player, char *msg)
{
	pthread_mutex_lock(player->bufforLock);
	arraylist_add(player->buffor, msg);
	pthread_mutex_unlock(player->bufforLock);
	pthread_cond_signal(player->bufforCondition);
}

void broadcast(Player *player, char *msg)
{
	DBG;
	fprintf(stderr,"%s: Broadcast\n", player->nick);
	for (int i=0; i<arraylist_size(player->players); i++) {
		Player* p = arraylist_get(player->players, i);
		sentFeedback(p, msg);		
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

int isCorridorEmpty(Player* p, int pos)
{
	for (int i=0; i<arraylist_size(p->players); i++) {
		Player* p = arraylist_get(p->players, i);
		if (p->position == pos) return FALSE;
	}
	return TRUE;
}

int isValidMove(Player *p, int newPosition)
{
	if (newPosition < 0) return FALSE;
	if (p->map->map[newPosition] == WALL) return FALSE;
	if (p->map->map[newPosition] == ROOM) return TRUE;	
	
	return isCorridorEmpty(p, newPosition);
}

int movePlayer(Player *player, int newPosition)
{
	Map *map = player->map;
	int ret = 0;
	int currentPosition = player->position;
	pthread_mutex_lock(&map->mutexs[MIN(newPosition, currentPosition)]);
	pthread_mutex_lock(&map->mutexs[MAX(newPosition, currentPosition)]);
	
	if (isValidMove(player, newPosition)) {
		player->position = newPosition;		
	} else {
		ret = -1;
	}
	
	pthread_mutex_unlock(&map->mutexs[MIN(newPosition, currentPosition)]);
	pthread_mutex_unlock(&map->mutexs[MAX(newPosition, currentPosition)]);
	
	return ret;
}

int tryMovePlayer(Player *player, int newPosition)
{
	if ((newPosition < 0 || !isValidMove(player, newPosition)) 
		&& movePlayer(player, newPosition) != 0)
	{
		sentFeedback(player, ILLEGAL_MOVE);
		return FALSE;
	} else {	
		return TRUE;
	}
}

int up(Player *player, char* nil)
{
	DBG;
	fprintf(stderr,"%s: UP request\n", player->nick);
		
	int newPosition = player->position - player->map->width;

	if (tryMovePlayer(player, newPosition) == TRUE) {	
		broadcastWithNick(player, "Go up");
	}
		
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
	sentFeedback(player, ILLEGAL_MOVE);
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
