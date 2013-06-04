#include "server.h"

#include "gamelogic.h"

int sethandler( void (*f)(int), int sigNo)
{
	struct sigaction act;
	memset(&act, 0, sizeof(struct sigaction));
	act.sa_handler = f;
	if (-1==sigaction(sigNo, &act, NULL))
		return -1;
	return 0;
}

void usage(char *name)
{
	printf("USAGE: %s port\n", name);
}

int findPlayerIndexWithNick(arraylist *players, char nick[NICK_LENGTH])
{
	fprintf(stderr,"Search for %s\nPlayers count: %d\n", nick, arraylist_size(players));
	for (int i=0; i<arraylist_size(players); i++) {
		Player *p = (Player*)arraylist_get(players, i);
		if (strcmp(p->nick, nick) == 0) {
			return i;
		}
	}
	return -1;
}

int findPlayerIndex(Player *player)
{
	fprintf(stderr,"Search for %s\nPlayers count: %d\n", player->nick, arraylist_size(player->players));
	for (int i=0; i<arraylist_size(player->players); i++) {
		Player *p = (Player*)arraylist_get(player->players, i);
		if (strcmp(p->nick, player->nick) == 0) {
			return i;
		}
	}
	return -1;
}

Player* findPlayerWithNick(arraylist *players, char nick[NICK_LENGTH])
{
	int index = findPlayerIndexWithNick(players, nick);
	return arraylist_get(players, index);
}

void removePlayer(Player *player)
{
	pthread_mutex_lock(player->players->lock);
	fprintf(stderr,"%s will be removed\nPlayers count: %d\n", player->nick, arraylist_size(player->players));
	int index = findPlayerIndex(player);
	assert(index >= 0);
	disposePlayer(player);
	arraylist_remove(player->players, index);
	fprintf(stderr,"Players count: %d\n", arraylist_size(player->players));
	pthread_mutex_unlock(player->players->lock);
}

void* clientReader(void* data)
{
	Player *player = (Player*)data;
	int fd = player->descriptor;

	join(player, NULL);
	DBG;
	while (TRUE) {
		char msg[MSG_LENGTH] = {0};
		if (bulk_read(fd, &msg, MSG_LENGTH) < MSG_LENGTH) {
			fprintf(stderr,"client read problem\n");
			leftGame(player, NULL);
			removePlayer(player);
			pthread_exit(NULL);
		}
		fprintf(stderr,"%s > %s\n", player->nick, msg);

		char *ret;
		fptr f = actionFactory(msg, &ret);

		f(player, ret);

		DBG;
	}

	return 0;
}

void* clientWriter(void* data)
{
	Player *player = (Player*)data;
	int fd = player->descriptor;
	int play = 1;

	while (play) {
		pthread_mutex_lock(player->bufforLock);
		do {
			if (arraylist_size(player->buffor) > 0) {
				DBG;
				fprintf(stderr,"%s has %d pending messages \n", player->nick, arraylist_size(player->buffor));
				while (arraylist_size(player->buffor) != 0) {
					char *buf = (char*)arraylist_pop(player->buffor);
					if(bulk_write(fd, buf, MSG_LENGTH) < 0) {
						fprintf(stderr,"%s did not recive message\n", player->nick);
						play = 0;
					}
					fprintf(stderr,"%s < %s\n", player->nick, buf);
					buf = NULL;	//arraylist takes care of memory
				}
				break;
			} else {
				pthread_cond_wait(player->bufforCondition, player->bufforLock);
			}
		} while (1);
		pthread_mutex_unlock(player->bufforLock);
	}
	return 0;
}

Player* initializePlayer(int socket, char nick[NICK_LENGTH], Map *map, arraylist *players)
{
	Player *player;
	int att = rand() % MAX_ATTRIBUTE;
	int pos = getRandomRoom(map);

	fprintf(stderr,"Create new player\n");
	player = createPlayer(nick, att, pos, socket, players, map);
	showPlayerInfo(player);

	pthread_create(&player->reader,NULL,clientReader,player);
	pthread_detach(player->reader);
	pthread_create(&player->writer,NULL,clientWriter,player);
	pthread_detach(player->writer);

	return player;
}

int isNickValid(arraylist *players, char nick[NICK_LENGTH])
{
	fprintf(stderr,"Check if %s is already used\nPlayers count: %d\n", nick, arraylist_size(players));
	return findPlayerIndexWithNick(players, nick) == -1;
}

void addNewPlayer(int socket, arraylist *players, Map *map)
{
	fprintf(stderr,"Player #%d name: ", arraylist_size(players));

	char buf[MSG_LENGTH] = {0};
	strncpy(buf, "Nick: ", MSG_LENGTH);
	if (bulk_write(socket, buf, MSG_LENGTH) < 0) {
		fprintf(stderr,"player write problem");
	}

	char nick[NICK_LENGTH];
	if (bulk_read(socket, &nick, NICK_LENGTH) < 0) {
		fprintf(stderr,"nick read problem");
		return;
	}

	fprintf(stderr,"%s\n", nick);
	if (isNickValid(players, nick)) {
		Player *p = initializePlayer(socket, nick, map, players);
		pthread_mutex_lock(players->lock);
		arraylist_add(players, p);
		pthread_mutex_unlock(players->lock);
	} else {
		strncpy(buf, "Wrong nick\n", MSG_LENGTH);
		fprintf(stderr,"%s", buf);
		if (bulk_write(socket, buf, MSG_LENGTH) < 0) {
			fprintf(stderr,"player write problem");
		}
		if(TEMP_FAILURE_RETRY(close(socket))<0)ERR("close");
	}
}

void waitForPlayers(int sfd, uint32_t port, Map map)
{
	int nfd;
	struct sockaddr_in client;
	socklen_t clen;
	pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
	arraylist *players = arraylist_create();
	players->lock = &mutex;
	fprintf(stderr,"Waiting for clients:\n");

	while(TRUE) {

		if((nfd=accept(sfd, (struct sockaddr *)&client, &clen))<0) {
			if(EINTR==errno) continue;
			ERR("accept");
		}
		addNewPlayer(nfd, players, &map);
	}
	pthread_mutex_destroy(&mutex);
	arraylist_destroy(players);
}

int main(int argc, char** argv)
{
	if(argc != 2) {
		usage(argv[0]);
		return EXIT_FAILURE;
	}

	signal(SIGPIPE, SIG_IGN);

	Map m = readMapFromFile("sample.map");
	printMap(m);

	int socket, port = atoi(argv[1]);
	socket=bind_inet_socket(port, SOCK_STREAM);

	waitForPlayers(socket, port, m);

	if(TEMP_FAILURE_RETRY(close(socket))<0)ERR("close");

	deleteMap(&m);
	printMap(m);

	return EXIT_SUCCESS;
}

