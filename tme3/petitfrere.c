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
pid_t processusinitial;

void sig_hand(int sig){

}

int main (int argc, char* argv []) {
	int numbre = 0;
    pid_t pid_fils;
	pid_t pid_pere;	
	sigset_t sig_proc;
	struct sigaction action;
	sigemptyset(&sig_proc);
	action.sa_mask = sig_proc;
	action.sa_flags = 0;
	action.sa_handler = sig_hand;
	sigaction(SIGUSR1,&action,0);
	sigaction(SIGUSR2,&action,0);
	sigaddset(&sig_proc,SIGUSR1);
	sigaddset(&sig_proc,SIGUSR2);
	sigprocmask(SIG_SETMASK,&sig_proc,NULL);

	while(numbre < 3){
		if((pid_fils = fork()) == 0){
			if(numbre == 0){  
				monfrere = getpid();
				
				printf("fils 1 pid: %d ,fils 1 ppid: %d \n", getpid(),getppid());
				kill(getppid(),SIGUSR1);
				
			}
			if(numbre == 1){ 
				 pid_pere = getppid();
				if( pid_pere == monfrere){	
					printf("fils 1.1 pid: %d ,fils 1.1 ppid: %d \n", getpid(),getppid());
					kill(getppid(),SIGUSR1);
					exit(0);
				}
				else{
					           			
					printf("fils 2 pid: %d ,fils 2 ppid: %d,fils 2 frere: %d\n", getpid(),getppid(), pid_pere);
					kill(pid_pere,SIGUSR2); 
					
				}
			}
			if(numbre == 2){
				printf("fils 2.1 pid: %d ,fils 2.1 ppid: %d \n", getpid(),getppid());
				kill(getppid(),SIGUSR2);
				exit(0);
			}
				
		}
		else{
				if(numbre > 0){
					if(getpid()==pid_pere){
						
						sigfillset(&sig_proc);
						sigdelset(&sig_proc,SIGUSR1);
						sigsuspend(&sig_proc);
						sigfillset(&sig_proc);
						sigdelset(&sig_proc,SIGUSR2);
						sigsuspend(&sig_proc);
						return 0;
					}else{
						sigfillset(&sig_proc);
						sigdelset(&sig_proc,SIGUSR2);
						sigsuspend(&sig_proc);
						return 0;									
					}
				}
						
		}
			
		numbre++;
	}
		

	return 0;
}


