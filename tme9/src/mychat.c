#define _XOPEN_SOURCE 700
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <netdb.h>

pthread_t  tid[2];

void * serveur(void *sc)
{	
	int i;	
	 i = *((int*)(sc));
	
	char buffer[10];
	while(1){
		if ( recvfrom(i,buffer,sizeof(buffer),0,(struct sockaddr *)&exp,&fromlen) == -1) {
			perror("recvfrom"); exit(2);
		}
		printf("buffer %s",buffer);
	}

	
	pthread_exit((void*)0);
}

void * client(void *sc)
{	
	int i;	
	i = *((int*)(sc));
	while(1){
		char* message;
		fgets(message,sizeof(message),stdin);
		if ( sendto(i,message,sizeof(message),0,(struct sockaddr *)&exp,fromlen) == -1) {
		perror("sendto"); exit(2);
		}
	}
	
	pthread_exit((void*)0);
}

int main(int argc, char *argv[])
{
	
	struct sockaddr_in sin; /* Nom de la socket du serveur */
	
	int sc ;

	struct ip_mreq imr;
	imr.imr_multiaddr.s_addr = inet_addr(argv[1]);
	imr.imr_interface.s_addr = INADDR_ANY;

	if ((sc = socket(AF_INET,SOCK_DGRAM,0)) < 0) {
		perror("socket"); exit(1);
	}
	setsockopt(sc,IPPROTO_IP,IP_ADD_MEMBERSHIP,(char*)&imr,sizeof(struct ip_mreq));
	memset((char *)&sin,0, sizeof(sin));

	sin.sin_port = htons(argv[2]);
	sin.sin_family = AF_INET;
	
	
	if (bind(sc,(struct sockaddr *)&sin,sizeof(sin)) < 0) {
		perror("bind"); exit(2);
	}
	
	int* pt_ind;
	 pt_ind =(int*) malloc(sizeof (int));
	*pt_ind = sc;
	if(pthread_create(&(tid[0]),NULL,serveur,pt_ind)!=0){
			perror("pthread_creat \n");
			exit(1);	
		}

	if(pthread_create(&(tid[1]),NULL,client,pt_ind)!=0){
			perror("pthread_creat \n");
			exit(1);	
		}

	if(pthread_join(tid[0],(void**)&status)!=0){
		perror("pthread_join \n");
		exit(1);	
		}

	if(pthread_join(tid[1],(void**)&status)!=0){
		perror("pthread_join \n");
		exit(1);	
		}
	
	
	close(sc);
return(0);

}
