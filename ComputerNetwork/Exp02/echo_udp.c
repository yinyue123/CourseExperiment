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
    socklen_t len;
    int server;
    struct sockaddr_in saddr;
    struct sockaddr_in raddr;
    char msg[1500];
    char msg2[1500];
    int msglen;
    if(argc != 3 && argc != 4)
        prierr("Usage:\n \
                Server: s <port>\n \
                Client: c <ip> <port>");

    sockfd=socket(AF_INET,SOCK_DGRAM,0);
    if(!strcmp(argv[1],"s"))
        server=1;
    else if (!strcmp(argv[1],"c"))
        server=0;
    else
        prierr("Wroung input");
    memset(&saddr,0,sizeof(saddr));
    saddr.sin_family=AF_INET;
    if(server){
        saddr.sin_addr.s_addr=htonl(INADDR_ANY);
        saddr.sin_port=htons(atoi(argv[2]));
        if(bind(sockfd,(struct sockaddr*)&saddr,sizeof(saddr))==-1)
            prierr("bind err");
        while(1){
            if((msglen=recvfrom(sockfd,msg,sizeof(msg),0,(struct sockaddr*)&raddr,&len))<0)
                prierr("recvfrom error");
            msg[msglen]=0;
            printf("%s\n",msg);
            sprintf(msg2,"echo:%s",msg);
            strcpy(msg,msg2);
            printf("%s\n",msg);
            len=sizeof(raddr);
            sendto(sockfd,msg,sizeof(msg),0,(struct sockaddr*)&raddr,len);
        }
        close(sockfd);
    }else{
        saddr.sin_addr.s_addr=inet_addr(argv[2]);
        saddr.sin_port=htons(atoi(argv[3]));
        while(1){
            scanf("%s",msg);
            if(!strcmp(msg,"q"))
                close(sockfd);
            len=sizeof(saddr);
            sendto(sockfd,msg,sizeof(msg),0,(struct sockaddr*)&saddr,len);
            if((msglen=recvfrom(sockfd,msg,sizeof(msg),0,(struct sockaddr*)&raddr,&len))<0)
                prierr("recvfrom error");
            msg[msglen]=0;
            fputs(msg,stdout);
            //printf("%s\n",msg);
        }
    }
    return 0;
}
