#define _XOPEN_SOURCE 700
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#include <arpa/inet.h>
#include <netdb.h>



struct msg{
	int type;
	char charid[10];
	char charval[10];
};


int main(int argc, char *argv[])
{
	int portserv = atoi(argv[2]);
	struct msg message;
	struct sockaddr_in dest; 
	int sock ;
	int fromlen = sizeof(dest);
	struct addrinfo *result;
	if ((sock = socket(AF_INET,SOCK_DGRAM,0)) < 0) {
		perror("socket"); exit(1);
	}

	if(getaddrinfo(argv[1],NULL,NULL,&result)!=0){
		perror("getinfo");exit(1);
	}
	
	memset((char *)&dest,0, sizeof(dest));
	memcpy((void*)&dest.sin_addr,(void*)&((struct sockaddr_in*)result->ai_addr)->sin_addr,sizeof(dest));
	
	dest.sin_port = htons(portserv);
	dest.sin_family = AF_INET;
	
	char line[50];
	fgets(line,sizeof(line),stdin);
	sscanf(line,"%d %s %s",&message.type,message.charid,message.charval);
	if ( sendto(sock,(struct msg *)&message,sizeof(message),0,(struct sockaddr *)&dest,fromlen) == -1) {
		perror("sendto"); exit(2);
	}
	
	close(sock);
return(0);

}
