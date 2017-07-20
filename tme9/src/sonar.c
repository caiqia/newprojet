#define _XOPEN_SOURCE 700
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#include <arpa/inet.h>
#include <netdb.h>

pthread_t  tid[2];

char* message = "ping";

struct sockaddr_in exp;
int fromlen = sizeof(exp); 

void *test1(void *sc)
{
	int i;	
	 i = *((int*)(sc));
	
	char buffer[10];
	

	if ( recvfrom(i,buffer,sizeof(buffer),0,(struct sockaddr *)&exp,&fromlen) == -1) {
			perror("recvfrom"); exit(2);
		}
	pthread_exit((void*)0);
}

void *test2(void *sc)
{
	int i;	
	i = *((int*)(sc));
	
	if ( sendto(i,message,sizeof(message),0,(struct sockaddr *)&exp,fromlen) == -1) {
		perror("sendto"); exit(2);
	}
	pthread_exit((void*)0);
}

int main(int argc, char *argv[])
{
	
	struct sockaddr_in sin; /* Nom de la socket du serveur */
	
	int sc ;
	int on = 1;
	int *status;

	if ((sc = socket(AF_INET,SOCK_DGRAM,0)) < 0) {
		perror("socket"); exit(1);
	}
	if((setsockopt(sc,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on))==-1){
		perror("setsockopt");exit(1);	
	}
	if((setsockopt(sc,SOL_SOCKET,SO_BROADCAST,&on,sizeof(on))==-1){
		perror("setsockopt");exit(1);	
	}
	memset((char *)&sin,0, sizeof(sin));

	sin.sin_port = htons(9999);
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_BROADCAST;
	if (bind(sc,(struct sockaddr *)&sin,sizeof(sin)) < 0) {
		perror("bind"); exit(2);
	}
	
	int* pt_ind;
	 pt_ind =(int*) malloc(sizeof (int));
	*pt_ind = sc;
	if(pthread_create(&(tid[0]),NULL,test1,pt_ind)!=0){
			perror("pthread_creat \n");
			exit(1);	
		}

	if(pthread_create(&(tid[1]),NULL,test2,pt_ind)!=0){
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
