#ifndef _COMUNICATION_H_

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

int make_socket(int domain, int type);
int bind_inet_socket(uint16_t port,int type);
int connect_inet_socket(struct in_addr ip, int port, int type);
ssize_t bulk_write(int fd, const void *buf, size_t count);
ssize_t bulk_read(int fd, void  *buf, size_t count);

#define _COMUNICATION_H_
#endif
