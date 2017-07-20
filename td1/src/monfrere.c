#define _XOPEN_SOURCE 700
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>


pid_t monfrere ;
pid_t processusinitial ;


int main (int argc, char* argv []) {
	int numbre = 0;
    	pid_t pid;
	
	while(numbre < 3){
		if((pid = fork()) == 0){
			if(numbre == 0){
				monfrere = getpid();
				processusinitial =	getppid();
				printf("fils 1 pid: %d ,fils 1 ppid: %d \n", getpid(),getppid());
			}
			if(numbre == 1){
				if( getppid() == monfrere){	
					printf("fils 1.1 pid: %d ,fils 1.1 ppid: %d \n", getpid(),getppid());
					exit(0);
				}
				else{
					
					printf("fils 2 pid: %d ,fils 2 ppid: %d,fils 2 frere: %d\n", getpid(),getppid(),monfrere);
				}
			}
			if(numbre == 2){
				printf("fils 2.1 pid: %d ,fils 2.1 ppid: %d \n", getpid(),getppid());
				exit(0);
			}
				
		}
		else{
		
			if(numbre > 0){
				wait(NULL);
				return 0;			
			}
			
		}
		numbre++;
	}
		
	return 0;
}


