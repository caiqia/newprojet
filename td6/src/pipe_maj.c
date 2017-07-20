#define _POSIX_SOURCE 1
#define _REENTRANT
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <pthread.h>
#include <sys/wait.h>
#include <fcntl.h>

#define S_BUF 100


char buffer[100];

pid_t pid_fils;

int main(int argc, char**argv)
{
	int i;
    int k;
	int n;
	int tubeDesc[2];
	char message[100];
	if(pipe(tubeDesc)==-1){
		perror("pipe");
		exit(1);	
	}
	if((pid_fils = fork())==-1){
		perror("fork");
		exit(2);	
	}
	if(pid_fils == 0){
		
		while(1){
			if((k = read(tubeDesc[0],buffer,S_BUF))==-1){
			perror("read");	
			exit(3);		
			}else{				
				for(i=0;i<k;i++){
					message[i] = toupper(buffer[i]);					
				}		
			}			
			write(STDOUT_FILENO,message,k);
		}
			
	}else{
		
		while(1){
			if(( n= read(STDIN_FILENO,buffer,S_BUF))==-1){
			perror("read");	
			exit(3);		
			}else{
				
				if(write(tubeDesc[1],buffer,n)==-1){
				perror("write");
				exit(4);		
				}
			
			}
		
		wait(NULL);
		}
	}
	return 0;
}



