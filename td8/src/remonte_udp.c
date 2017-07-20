#define _XOPEN_SOURCE 700
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#include <arpa/inet.h>
#include <netdb.h>
#define PORTSERV 4567


int main(int argc, char *argv[])
{
	struct sockaddr_un addr;

	int sock;

	int k ;
	 pid_t pid_fils;
	int n = atoi(argv[1]);
	int valeuraleatoire;
	int val;
	int fromlen = sizeof(addr);
	if ((sock = socket(AF_UNIX, SOCK_DGRAM, 0)) == -1)
		{perror("Erreur creation socket");exit(1);}

	memset(&addr,0, sizeof(struct sockaddr_un));
	addr.sun_family = AF_UNIX;
	strcpy(addr.sun_path, "./MySock");
/* nommage */
	if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) == -1)
		{perror("Erreur au nommage");exit(2);}
	for(k=0;k<n;k++){
		if((pid_fils = fork()) == 0)
		{	
			valeuraleatoire = (int)(10*(float)rand () / RAND_MAX);
			printf("valeuraleatoire %d\n",valeuraleatoire);
			if (sendto(sock,&valeuraleatoire,sizeof(valeuraleatoire),0,(struct sockaddr *)&addr,fromlen) == -1) {
				perror("sendto"); exit(4);
			}
			exit(0);
		}
	}
	if ( recvfrom(sock,&val,sizeof(val),0,(struct sockaddr *)&addr,&fromlen) == -1) {
				perror("recvfrom"); exit(2);
			}
		
			
	for(k=0;k<n;k++){
		wait(NULL);
		printf("val: %d\n",val);
	}
	unlink("./MySock");
	close(sock);
	
	
return(0);

}
