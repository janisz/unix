#ifndef _CONSTANS_H_

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>


#define WALL 'X'
#define CORRIDOR 'C'
#define ROOM 'H'

#define UP		"up"
#define DOWN	"down"
#define LEFT	"left"
#define RIGHT	"right"
#define SHOW	"show"
#define ATTACK	"attack"

#define MAX_ATTRIBUTE 100;

#define NICK_LENGTH 20
#define MSG_LENGTH (10 + NICK_LENGTH)

#define MAP_SIZE(m) (((m).width)*((m).height))

#define TRUE (!FALSE)
#define FALSE (0)

#define DBG			(fprintf(stderr,"%s:%d:\t",__FILE__,__LINE__)); 

#define ERR(source) (fprintf(stderr,"%s:%d\n",__FILE__,__LINE__),\
                     perror(source),kill(0,SIGKILL),\
		     		     exit(EXIT_FAILURE))

#define MIN(a, b)  (((a) < (b)) ? (a) : (b))
#define MAX(a, b)  (((a) > (b)) ? (a) : (b))

#define _CONSTANS_H_
#endif
