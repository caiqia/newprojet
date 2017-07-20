#define _XOPEN_SOURCE 700
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void sig_hand(int sig){
	printf("signal recu %d \n",sig);
}

int main (int argc, char* argv []) {
	
	int i = 0 ;
	int numbre = 0;
	int retour_fils;
    pid_t pid_fils;
	pid_t pid_pere;
	
	sigset_t sig_proc;
	struct sigaction action;
	sigemptyset(&sig_proc);
	action.sa_mask = sig_proc;
	action.sa_flags = 0;
	action.sa_handler = sig_hand;
	sigaction(SIGUSR1,&action,0);
	

	sigaddset(&sig_proc,SIGUSR1);
	
	sigprocmask(SIG_SETMASK,&sig_proc,NULL);
	
	
		if((pid_fils = fork()) == 0)
		{
			sleep(1);
			printf("fils apres sleep \n");
			sigfillset(&sig_proc);
				sigdelset(&sig_proc,SIGUSR1);
				sigsuspend(&sig_proc);
				printf("reprise fils \n");
			
		}
		else {
				kill(pid_fils,SIGUSR1);
				wait(NULL);
				printf("fin pere\n");
 
			}
	return 0;
}

