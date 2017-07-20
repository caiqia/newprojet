
#define _XOPEN_SOURCE 700
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <signal.h>

#define n 5

pid_t chaine[n];

void sig_hand(int sig){
	
}

int main (int argc, char* argv []) {

	int i = 0 ;
	int numbre = 0;
    pid_t pid_fils;
	
	int valeuraleatoire;
	sigset_t sig_proc;
	struct sigaction action;
	sigemptyset(&sig_proc);
	action.sa_mask = sig_proc;
	action.sa_flags = 0;
	action.sa_handler = sig_hand;
	sigaction(SIGUSR1,&action,0);
	sigaddset(&sig_proc,SIGUSR1);
	sigprocmask(SIG_SETMASK,&sig_proc,NULL);

	while ( numbre <= n) 
	{
		if((pid_fils = fork()) == 0)
		{		   
			if(numbre == n){
				sigfillset(&sig_proc);
				sigdelset(&sig_proc,SIGUSR1);
				sigsuspend(&sig_proc);
	
				chaine[n] = getpid();
				while(i <= n){
					printf("processus %d : pid %d \n", i,chaine[i]);
					i++;
				}								
				
			}else{
				chaine[numbre] = getppid();						
			}	
		}
		 else {					
			if(numbre == n){			
				printf("pid : %d , pid de pere : %d , pas de fils  \n", getpid(),getppid());
				kill(pid_fils,SIGUSR1);
			}
				
			if(numbre == 0){
					printf("pid : %d , pas de pere , pid de fils : %d  \n", getpid(),pid_fils);		
			}else{
				
				printf("pid : %d , pid de pere : %d , pid de fils : %d  \n", getpid(),getppid(),pid_fils);
						
			}
        	return 0;
		}
		numbre++;
		
 
	}
	return 0;
}

