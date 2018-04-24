#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <time.h>

int prierr(char *msg){
    printf("%s\n",msg);
    exit(1);
    return 0;
}

int main(int argc,char** argv){
    int sockfd;
    int port;
    int server;
    struct sockaddr_in addr;
    char msg[2000];
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
            time_t timep;
            time(&timep);
            sprintf(msg,"Time now:%s\n",asctime(gmtime(&timep)));
            printf("send %s\n",msg);
            write(cliaddr,msg,sizeof(msg));
            close(cliaddr);
        }
        close(sockfd);
    }else{
        addr.sin_addr.s_addr=inet_addr(argv[2]);
        addr.sin_port=htons(atoi(argv[3]));
        if(connect(sockfd,(struct sockaddr*)&addr,sizeof(addr))==-1)
            prierr("connect error!");
        int rcvlen;
	char *p=msg;
	do{
		rcvlen=read(sockfd,p,2000);
		if(rcvlen==-1)
            		printf("recv error\n");
		else if(rcvlen==0)
			printf("connection closed\n");
		p=(char *)(p+sizeof(char)*rcvlen);
        }while(rcvlen>0);
        printf("recv %s\n",msg);
        close(sockfd);
    }


    return 0;
}
