#define _POSIX_SOURCE 1
#define _REENTRANT
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/stat.h>
#include <errno.h>
#include <pthread.h>
#include <sys/wait.h>
#include <fcntl.h>

#define S_BUF 100


char buffer[S_BUF];



int main(int argc, char**argv)
{
	int i;
	int n;
    int fd_read;
	int fd_write;

	char message[100];
 
	while(1){
		if((fd_write = open(argv[1],O_WRONLY))==-1){
			perror("open");	
			exit(3);		
		}
		if(( n= read(STDIN_FILENO,buffer,S_BUF))==-1){
			perror("read");	
			exit(3);		
		}
		if(write(fd_write,buffer,n)==-1){
					perror("write");
					exit(4);		
		}		
		
	}
	return 0;
}





