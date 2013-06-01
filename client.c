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
#include <pthread.h>

#include "comunication.h"
#include "constans.h"

#define ERR(source) (fprintf(stderr,"%s:%d\n",__FILE__,__LINE__),\
                     perror(source),kill(0,SIGKILL),\
		     		     exit(EXIT_FAILURE))

void usage(char *name)
{
	fprintf(stderr,"USAGE: %s ip port\n",name);
}

void parse_arguments(int argc, char** argv,	struct in_addr *addr, int *port )
{
	if(argc != 3) {
		usage(argv[0]);
		exit(EXIT_FAILURE);
	}

	if ((*port = atoi(argv[2])) < 0) {
		usage(argv[0]);
		exit(EXIT_FAILURE);
	}

	if (inet_aton(argv[1], addr) == 0) {
		usage(argv[0]);
		exit(EXIT_FAILURE);
	}
}

void* clientReader(void* data)
{
	int fd = *((int*)data);

	while (1) {
		char msg[MSG_LENGTH] = {0};
		if (bulk_read(fd, &msg, MSG_LENGTH) < MSG_LENGTH) {
			fprintf(stderr,"read problem");
			printf("\n\t ** Game Over ** \n");
			exit(0);
		}
		printf("%s\n", msg);
	}

	return 0;
}

int main(int argc, char** argv)
{
	int socket, port;
	struct in_addr ifaddr;

	parse_arguments(argc, argv, &ifaddr, &port);

	socket = connect_inet_socket(ifaddr, port, SOCK_STREAM);

	pthread_t reader;
	pthread_create(&reader,NULL,clientReader,&socket);
	pthread_detach(reader);

	char nick[NICK_LENGTH];
	scanf("%s", nick);

	if(bulk_write(socket, nick, NICK_LENGTH) < NICK_LENGTH) {
		fprintf(stderr,"server did not recive nick");
	}

	char msg[MSG_LENGTH] = {0};
	while (1) {

		if ((fgets (msg, MSG_LENGTH, stdin)) == NULL) {
			fprintf(stderr,"Readline problem");
		}

		if(bulk_write(socket, msg, MSG_LENGTH) < MSG_LENGTH) {
			fprintf(stderr,"problem with sending data to server");
			printf("Game ended");
			break;
		}
	}

	if((TEMP_FAILURE_RETRY(close(socket)))<0) {
		ERR("close");
	}

	return EXIT_SUCCESS;
}
