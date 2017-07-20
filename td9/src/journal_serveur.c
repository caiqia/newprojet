#define _XOPEN_SOURCE 700


/** Client emetteur de fichiers pas TCP **/

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
   
	fd_set mselect;
	FD_ZERO(&mselect);
	int i;
	char host[64];
	int max=0;
    int sc[5];
	int scom;
	int fd;
	int fromlen = sizeof(exp);
	for(i=0;i<argc-1;i++){
		if ((sc[i] = socket(AF_INET,SOCK_STREAM,0)) == -1) {
		    perror("socket");
		    exit(1);
    	}
		memset((char*)&sin,0,sizeof(sin));
		sin.sin_port = htons(atoi(argv[i+1]));
		sin.sin_family = AF_INET;
		sin.sin_addr.s_addr = htonl(INADDR_ANY);
		if(bind(sc[i],(struct sockaddr *)&sin,sizeof(sin))<0){
			perror("bind");exit(1);	
		}
		listen(sc[i],1);
		if(sc[i]>max){
			max = sc[i];
			printf("sci%d\n",sc[i]);			
		}
		
	}
	printf("%d\n",max);	
		while(1){
			for(i=0;i<argc-1;i++){
				FD_SET(sc[i],&mselect);			
			}
			if((select(max+1,&mselect,NULL,NULL,NULL))==-1){
				perror("select"); exit(1);			
			}
			for(i=0;i<argc-1;i++){
				if(FD_ISSET(sc[i],&mselect)){
					if((scom=accept(sc[i],(struct sockaddr *)&exp,&fromlen))==-1){
						perror("accept"); exit(2);				
					}		
					if((fd = open("cx.log",O_WRONLY|O_CREAT,0600)) ==-1){
						perror("open error");
						exit(1);
					}
					if((getnameinfo((struct sockaddr *)&exp,sizeof(exp),host,sizeof(host),NULL,NULL,0))!=0){
						perror("getnameinfo");exit(1);					
					}
					printf("%s\n",host);
					if((write(fd,host,sizeof(host)))== -1){
						perror("write error");
						exit(1);
					}
					shutdown(scom,2);
					close(scom);	
				}
				
			}
		
	}
	for(i=0;i<argc-1;i++){
		close(sc[i]);
	}

    return 0;
}
