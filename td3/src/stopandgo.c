#define _XOPEN_SOURCE 700
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#define n 5

pid_t chaine[n];

void sig_hand(int sig){
	
}

int main (int argc, char* argv []) {

	int i = 0 ;
	int numbre = 0;
	int retour_fils;
    pid_t pid_fils;
	
	sigset_t sig_proc;
	struct sigaction action;
	sigemptyset(&sig_proc);
	action.sa_mask = sig_proc;
	action.sa_flags = 0;
	action.sa_handler = sig_hand;
	sigaction(SIGCONT,&action,0);
	sigaction(SIGCHLD,&action,0);


	sigaddset(&sig_proc,SIGCONT);
	sigaddset(&sig_proc,SIGCHLD);
	sigprocmask(SIG_SETMASK,&sig_proc,NULL);
	
	while ( numbre <= n) 
	{
		if((pid_fils = fork()) == 0)
		{
			
			if(numbre == n){
					chaine[n] = getpid();
					while(i <= n){
						printf("processus %d : pid %d \n", i,chaine[i]);
						i++;				
					}	
					kill(getpid(),SIGSTOP);	
					sigfillset(&sig_proc);
					sigdelset(&sig_proc,SIGCONT);
					sigsuspend(&sig_proc);
					
			}else{		
				chaine[numbre] = getppid();
			}
			
		}
		 else {

			sigfillset(&sig_proc);
			sigdelset(&sig_proc,SIGCHLD);	
			sigsuspend(&sig_proc);
			
			if(numbre == n){
				printf("pid : %d , pid de pere : %d , pas de fils  \n", getpid(),getppid());
				kill(getpid(),SIGSTOP);
			}
				
			if(numbre == 0){
					printf("pid : %d , pas de pere , pid de fils : %d  \n", getpid(),pid_fils);	
					printf("Tous les descendants sont suspendus");
					
						
			}else{
				
				printf("pid : %d , pid de pere : %d , pid de fils : %d  \n", getpid(),getppid(),pid_fils);
				kill(getpid(),SIGSTOP);			
			}
        	
			
			kill(pid_fils,SIGCONT);
			return 0;
			
		}
		numbre++;
 
	}
	return 0;
}

