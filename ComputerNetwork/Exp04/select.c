#include <stdio.h>
#include <stdlib.h>
//#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <string.h>
#define PORT 8000

int prierr(char *msg){
    printf("%s\n",msg);
    exit(1);
    return 0;
}

int main(int argc,char **argv)
{
	int listenfd,connfd,udpfd,nready;
	char msg[2000];
	pid_t childpid;
	fd_set rset;
	int rcvlen;
	socklen_t len;
	const int on=1;
	struct sockaddr_in cliaddr,servaddr;

	if((listenfd=socket(AF_INET,SOCK_STREAM,0))==-1)
		prierr("tcp socket error");

	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family=AF_INET;
	servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
	servaddr.sin_port=htons(PORT);

	setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
	if(bind(listenfd,(struct sockaddr*)&servaddr,sizeof(servaddr))==-1)
		prierr("tcp bind error");

	listen(listenfd,10);

	if((udpfd=socket(AF_INET,SOCK_DGRAM,0))==-1)
		prierr("upd socket error");

	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family     =AF_INET;
	servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
	servaddr.sin_port       =htons(PORT);

	if(bind(udpfd,(struct sockaddr*) &servaddr,sizeof(servaddr))==-1)
		prierr("udp bind error");


	FD_ZERO(&rset);
	while(1){
		FD_SET(listenfd,&rset);
		FD_SET(udpfd,&rset);
		if((nready=select((listenfd>udpfd)?listenfd+1:udpfd+1
						,&rset,NULL,NULL,NULL)) < 0){
		}

		if(FD_ISSET(listenfd,&rset)){
			len=sizeof(cliaddr);
			if((connfd=accept(listenfd,(struct sockaddr*) &cliaddr,&len))==-1)
				prierr("accept error");
			
			if((childpid=fork())==0){
				close(listenfd);
				if((rcvlen=recv(connfd,msg,sizeof(msg),0))==-1)
					prierr("recv error");
				printf("tcp recv:%s\n",msg);
				send(connfd,msg,rcvlen,0);
				printf("tcp send:%s\n",msg);
				exit(0);
			}
			close(connfd);
		}

		if(FD_ISSET(udpfd,&rset)){
			len=sizeof(cliaddr);
			if((rcvlen=recvfrom(udpfd,msg,sizeof(msg),0,(struct sockaddr*) &cliaddr,&len))==-1)
				prierr("recvfrom error");
			printf("udp recv:%s\n",msg);
			sendto(udpfd,msg,rcvlen,0,(struct sockaddr*) &cliaddr,len);
			printf("udp send:%s\n",msg);
		}
	}
}
