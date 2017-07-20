#define _POSIX_SOURCE 1
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>
#define SIZE_TAMPON 100

char tampon[SIZE_TAMPON];
int status;


int main(int argc, char* argv[])
{
    int fd1;
	int n;
	int i = 0;
	
	if((fd1 = open(argv[1],O_RDWR|O_CREAT,0600))==-1){
		  perror("open error");
		  return errno;
	}
	
	if((write(fd1,argv[3], 50))==-1){
		 perror("write error");
		 return errno;
	}
	printf("suite des mots %s\n",argv[3]);
	while(i< strlen(argv[3])){			
		if(argv[3][i]!=' '){		
			i++;
		}else{
			break;		
		}
	}
	if((lseek(fd1,0,SEEK_SET))==-1){
		perror("seek");
		return errno;
	}
	
	if((n = read(fd1,tampon,i+1))<=0){
		perror("read error");
		return errno;
	}

	printf("suite des mots %s\n",tampon);
		
 	if((write(fd1,argv[2], strlen(argv[2])))==-1){
		perror("write error");
		return errno;
	}

return 0;

}
