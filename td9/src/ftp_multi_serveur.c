#define _XOPEN_SOURCE 700
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#include <arpa/inet.h>
#include <netdb.h>



int main(int argc, char *argv[])
{
	struct sockaddr_in sin;
	struct sockaddr_in exp; 
	int sc;
	int scom;
	char* nomfich;
	int fromlen = sizeof(exp);
	if((sc = socket(AF_INET,SOCK_STREAM,0))<0){
		perror("socket");exit(1);	
	}
	memset((char*)&sin,0,sizeof(sin));
	sin.sin_port = htons(atoi(argv[1]));
	sin.sin_family = AF_INET;
	if(bind(sc,(struct sockaddr *)&sin,sizeof(sin))<0){
		perror("bind");exit(1);	
	}
	listen(sc,5);
	for(;;)
	if((scom = accept(sc,(struct sockaddr *)&exp,&fromlen))==-1){
			perror("accept");exit(3);		
		}
	
	if(read(scom,nomfich,sizeof(nomfich))==-1){
		perror("read"); exit(1);
	}
	printf("%s",nomfich);	
	shutdown(scom,2);
	close(scom);
	
	close(sc);
return 0;

}
