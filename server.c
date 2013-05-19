#define _GNU_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define ERR(source) (fprintf(stderr,"%s:%d\n",__FILE__,__LINE__),\
                     perror(source),kill(0,SIGKILL),\
		     		     exit(EXIT_FAILURE))

#define WALL 'X'
#define CORRIDOR 'C'
#define ROOM 'H'

#define UP		"w"
#define DOWN	"s"
#define LEFT	"a"
#define RIGHT	"d"
#define Show	"show"
#define Attack	"attack"

#define NICK_LENGTH 20

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


int main(int argc, char** argv)
{
	return EXIT_SUCCESS;
}

