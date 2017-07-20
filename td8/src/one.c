#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <time.h>
#include <pthread.h>


int main(int argc, char *argv[])
{

	int j,k;	

int i;
	char buffer[1000];
	char chemin[1000];
	FILE *fich;
	if((fich = fopen("fich1.txt", "r+"))==0){
		printf("fopen\n");
	}
	//setlinebuf(fich);
	for(i=0;i<3;i++){
	if((fgets(buffer, sizeof(buffer), fich))<=0){
			printf("%s\n","fgets");

		}
		printf("buffer %s\n",buffer);
	  if ((strncmp(buffer, "test", 4))!=0){
	  		printf("%s\n","GET error");
	  } 
	  for (j=4; buffer[j]==' '; j++);
    for (k=0; buffer[k] && buffer[k]!=' ' && k<sizeof(buffer); k++, j++){
		chemin[k] = buffer[j];
	}
	chemin[k] = 0;
	for (; buffer[j]==' '; j++);
	if ((strncmp(buffer+j, "HTTP/1.1", 8))!=0){
			printf("%s\n","HTTP error");
	}
	for (; buffer[j] && buffer[j]!='\n'; j++);
	if((fgets(buffer, sizeof(buffer), fich))<=0){
			printf("%s\n","fgets");

		}
	printf("buffer %s\n",buffer);
	if ((strncmp(buffer, "deux wang", 9))!=0){
			printf("%s\n","HTTP error");
	}
	
	printf("%s\n",chemin);
}
		
return(0);

}
