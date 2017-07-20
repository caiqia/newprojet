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
	int portserv = atoi(argv[1]);
	struct sockaddr_in sin; /* Nom de la socket du serveur */
	struct sockaddr_in exp; /* Nom de l'expediteur */
	struct msg message;
	int sc ;
	int fromlen = sizeof(exp);


	if ((sc = socket(AF_INET,SOCK_DGRAM,0)) < 0) {
		perror("socket"); exit(1);
	}
	
	memset((char *)&sin,0, sizeof(sin));

	sin.sin_port = htons(portserv);
	sin.sin_family = AF_INET;
	/* nommage */
	if (bind(sc,(struct sockaddr *)&sin,sizeof(sin)) < 0) {
		perror("bind"); exit(2);
	}
	
	
	if ( recvfrom(sc,(struct msg*)&message,sizeof(message),0,(struct sockaddr *)&exp,&fromlen) == -1) {
		perror("recvfrom"); exit(2);
	}

	printf("id  %s\n",(message.charid));
	printf("val  %s\n",(message.charval));
	printf("type  %d\n",message.type);
	
	close(sc);
return(0);

}
