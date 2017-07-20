#define _XOPEN_SOURCE 700


#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>

int main(int argc, char *argv[])
{
    struct sockaddr_in sin; 
	struct sockaddr_in exp; 
   	char nomfich[10];
	int i;
	char host[64];
	int max=0;
    int sc;
	int scom;
	int fd;
	int fromlen = sizeof(exp);
	if((sc = socket(AF_INET,SOCK_STREAM,0))<0){
		perror("socket");exit(1);	
	}
	
	memset((char*)&sin,0,sizeof(sin));
	sin.sin_port = htons(7100);
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = htonl(INADDR_ANY); 
	if(bind(sc,(struct sockaddr *)&sin,sizeof(sin))<0){
		perror("bind");exit(1);	
	}
	listen(sc,5);
	while(1){
		if((scom=accept(sc,(struct sockaddr *)&exp,&fromlen))==-1){
			perror("accept"); exit(2);				
		}		
		if((getnameinfo((struct sockaddr *)&exp,sizeof(exp),host,sizeof(host),NULL,NULL,0))!=0){
			perror("getnameinfo");exit(1);					
		}
		printf("%s\n",host);
		if(read(scom,&nomfich,sizeof(nomfich))==-1){
		perror("read"); exit(1);
		}
		printf("%s\n",nomfich);
		shutdown(scom,2);
		close(scom);	
		}
				
		close(sc);

    return 0;
}
