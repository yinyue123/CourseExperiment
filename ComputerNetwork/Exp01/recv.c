#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int prierr(char *msg){
	printf("%s\n",msg);
	exit(1);
	return 0;
}

int recvn(int socket,char* buf,int len){
	int msglen;
	int left=len;
	while(msglen>0){
		msglen=read(socket,buf,left);
		if(msglen<0)
			printf("recv data err\n");
		else if(msglen==0)
			return len-left;
		left-=msglen;
		buf+=msglen;
	}
	return len-left;
}

int recvl(int socket,char* buf){
	int len;
	int rcvlen;
	char *p=buf;
	do{
		rcvlen=read(socket,p,sizeof(buf));
		if(rcvlen==-1)
			printf("recv error\n");
		else if(rcvlen==0)
			printf("connection closed\n");
		len+=rcvlen;
		p+=rcvlen;
		printf("%s",p);
	}while(rcvlen>0);
	return len;
}

int recvlen(int socket,char* buf){
	int len;
	int rcvlen;
	char *p=buf;

	do{
		rcvlen=read(socket,p,1);
		if(rcvlen==-1)
			printf("recv len\n");
		len++;
		p++;
	}while(*p!='\n');
	return len;
}

int main(int argc,char** argv){
	int sockfd;
	int port;
	int server;
	int rcvlen;
	struct sockaddr_in addr;
	char msg[2000];
	char msg2[2000];
	int choice=0;
	if(argc != 3 && argc != 4)
		prierr("Usage:\n \
				Server: s <port>\n \
				Client: c <ip> <port>");

	sockfd=socket(AF_INET,SOCK_STREAM,0);
	if(!strcmp(argv[1],"s"))
		server=1;
	else if (!strcmp(argv[1],"c"))
		server=0;
	else
		prierr("Wroung input");
	memset(&addr,0,sizeof(addr));
	addr.sin_family=AF_INET;
	if(server){
		addr.sin_addr.s_addr=htonl(INADDR_ANY);
		addr.sin_port=htons(atoi(argv[2]));
		if(bind(sockfd,(struct sockaddr*)&addr,sizeof(addr))==-1)
			prierr("bind err");
		if(listen(sockfd,5)==-1)
			prierr("listen err");
		while(1){
			int cliaddr;
			socklen_t clilen=sizeof(cliaddr);
			cliaddr=accept(sockfd,(struct sockaddr*)&cliaddr,&clilen);
			if(cliaddr==-1)
				prierr("accept err");
			//rcvlen=read(cliaddr,msg,sizeof(msg));			
///*
			choice=0;//choice one you want to use
			switch(choice){
				case 0:
					printf("Using recvn:\n");
					rcvlen=recvn(cliaddr,msg,10);
					break;
				case 1:
					printf("Using recvl:\n");
					rcvlen=recvl(cliaddr,msg);
					break;
				case 2:
					printf("Using recvlen:\n");
					rcvlen=recvlen(cliaddr,msg);
					break;
			}
//*/
			printf("recv:%s\n",msg);
			//printf("recv len:%d\n",rcvlen);
			sprintf(msg2,"echo:%s",msg);
			if(!strcmp(msg,"q"))
				prierr("Bye bye");
			strcpy(msg,msg2);
			write(cliaddr,msg,rcvlen);
			printf("send:%s\n",msg);
			close(cliaddr);
		}
		close(sockfd);
	}else{
		addr.sin_addr.s_addr=inet_addr(argv[2]);
		addr.sin_port=htons(atoi(argv[3]));
		if(connect(sockfd,(struct sockaddr*)&addr,sizeof(addr))==-1)
			prierr("connect error!");
		scanf("%s",&msg);
		write(sockfd,msg,sizeof(msg));
		int rcvlen;
		char *p=msg;
		do{
			rcvlen=read(sockfd,p,sizeof(msg));
			if(rcvlen==-1)
				printf("recv error\n");
		//	else if(rcvlen==0)
		//		printf("connection closed\n");
			p=(char *)(p+sizeof(char)*rcvlen);
		}while(rcvlen>0);
		printf("recv %s\n",msg);
		close(sockfd);
	}


	return 0;
}
