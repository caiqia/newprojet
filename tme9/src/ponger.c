#define _XOPEN_SOURCE 700
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#include <arpa/inet.h>
#include <netdb.h>

char* message = "pong";

int main(int argc, char *argv[])
{
	
	struct sockaddr_in sin; /* Nom de la socket du serveur */
	struct sockaddr_in exp;
	char buffer[10];
	int sock ;
	int fromlen = sizeof(dest);
	if ((sock = socket(AF_INET,SOCK_DGRAM,0)) < 0) {
		perror("socket"); exit(1);
	}
	
	memset((char *)&sin,0, sizeof(sin));

	sin.sin_port = htons(9999);
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;
	if (bind(sc,(struct sockaddr *)&sin,sizeof(sin)) < 0) {
		perror("bind"); exit(2);
	}

	if ( recvfrom(sock,buffer,sizeof(buffer),0,(struct sockaddr *)&exp,&fromlen) == -1) {
			perror("recvfrom"); exit(2);
		}

	if ( sendto(sock,message,sizeof(message),0,(struct sockaddr *)&exp,fromlen) == -1) {
		perror("sendto"); exit(2);
	}
	
	close(sc);
return(0);

}
