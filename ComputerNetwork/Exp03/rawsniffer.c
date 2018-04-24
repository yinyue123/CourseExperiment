#include <stdio.h>
#include <errno.h> 
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h> 
#include <linux/in.h>
#include <linux/if_ether.h>

int main(int argc,char **argv){
	int sock,n;
	char buffer[2048];
	unsigned char *iphead,*ethhead;
	if((sock=socket(PF_PACKET,SOCK_RAW,htons(ETH_P_IP)))<0){
		perror("socket");
		return 1;
	}
	while(1){
		n = recvfrom(sock,buffer,2048,0,NULL,NULL);
		if(n<42){
			perror("recvfrom():");
			printf("Incomplete packet(errno is %d)\n",
					errno);
			close(sock);
			return 0;
		}

		iphead = buffer+14; /* Skip Ethernet header */
		if(*iphead==0x45){ /* Double check for IPv4
				      * and no options present */
			if(iphead[9]==6 && (
						(iphead[20]<<8)+iphead[21]==21 || 
						(iphead[22]<<8)+iphead[23]==21)
					){
				printf("%d bytes read\n",n);
				printf("-----\n");
/*
				ethhead = buffer;
				printf("Source MAC address: "
						"%02x:%02x:%02x:%02x:%02x:%02x\n",
						ethhead[0],ethhead[1],ethhead[2],
						ethhead[3],ethhead[4],ethhead[5]);
				printf("Destination MAC address: "
						"%02x:%02x:%02x:%02x:%02x:%02x\n",
						ethhead[6],ethhead[7],ethhead[8],
						ethhead[9],ethhead[10],ethhead[11]);
*/
				iphead = buffer+14; /* Skip Ethernet header */
				printf("Source host %d.%d.%d.%d\n",
						iphead[12],iphead[13],
						iphead[14],iphead[15]);
				printf("Dest host %d.%d.%d.%d\n",
						iphead[16],iphead[17],
						iphead[18],iphead[19]);
				printf("Source,Dest ports %d,%d\n",
						(iphead[20]<<8)+iphead[21],
						(iphead[22]<<8)+iphead[23]);
				printf("Layer-4 protocol %d\n",iphead[9]);
				//*(buffer+n)=0;
				switch(iphead[9]){
					case 6://tcp
						printf("TCP protocol,data:\n%s",buffer+14+20+20);
						break;
					case 17://udp
						printf("UDP protocol,data:\n%s",buffer+14+20+8);
						break;				
				}
				printf("-----\n");
			}
		}
	}

}
