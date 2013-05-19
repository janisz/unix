#include "comunication.h"

int make_socket(int domain, int type)
{
	int sock;
	sock = socket(domain,type,0);
	if(sock < 0) ERR("socket");

	return sock;
}

int bind_inet_socket(uint16_t port,int type)
{
	struct sockaddr_in addr;
	int socketfd,t=1;
	socketfd = make_socket(PF_INET,type);
	memset(&addr, 0, sizeof(struct sockaddr_in));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	if (setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR,&t, sizeof(t))) ERR("setsockopt");
	if(bind(socketfd,(struct sockaddr*) &addr,sizeof(addr)) < 0)  ERR("bind");
	if(SOCK_STREAM==type)
		if(listen(socketfd, BACKLOG) < 0) ERR("listen");

	return socketfd;
}

int connect_inet_socket(struct in_addr ip, int port, int type)
{
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr = ip;
	int socketfd;
	socketfd = make_socket(PF_INET,type);
	if (connect(socketfd,(struct sockaddr*) &addr, sizeof(addr)) < 0) {
		if (errno != EINTR) ERR("connect");
		else {
			fd_set wfds ;
			int status;
			socklen_t size = sizeof(int);
			FD_ZERO(&wfds);
			FD_SET(socketfd, &wfds);
			if (TEMP_FAILURE_RETRY(select(socketfd+1, NULL, &wfds, NULL, NULL)) < 0) {
				ERR("select");
			}
			if (getsockopt(socketfd, SOL_SOCKET, SO_ERROR, &status,&size)<0) {
				ERR("getsockopt");
			}
			if(0!=status) ERR("connect");
		}
	}
	return socketfd;
}

ssize_t bulk_write(int fd, const void *buf, size_t count)
{
	int c;
	size_t len=0;
	do {
		c=TEMP_FAILURE_RETRY(write(fd,buf,count));
		if(c<0) return c;
		(*(char*)&buf) +=c;
		len+=c;
		count-=c;
	} while(count>0);

	return len;
}

ssize_t bulk_read(int fd, void  *buf, size_t count)
{
	int c;
	ssize_t len=0;
	do {
		c=TEMP_FAILURE_RETRY(read(fd,buf,count));
		if(c<0) return c;
		if(0==c) return len;
		(*(char*)&buf) +=c;
		len+=c;
		count-=c;
	} while(count>0);
	return len ;
}
