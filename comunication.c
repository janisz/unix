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
