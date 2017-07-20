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

void sig_hand(int sig){

}

 
pid_t pid_pere;

int main (int argc, char* argv []) {
	
	int i = 0 ;
	int numbre = 0;
	
    pid_t pf1;
	pid_t pf2;
	
	sigset_t sig_proc;
	struct sigaction action;
	sigemptyset(&sig_proc);
	action.sa_mask = sig_proc;
	action.sa_flags = 0;
	action.sa_handler = sig_hand;
	sigaction(SIGUSR1,&action,0);
	sigaction(SIGUSR2,&action,0);
	sigaction(SIGCHLD,&action,0);

	sigaddset(&sig_proc,SIGUSR1);
	sigaddset(&sig_proc,SIGUSR2);
	sigaddset(&sig_proc,SIGCHLD);
	sigprocmask(SIG_SETMASK,&sig_proc,NULL);
	
	
		if((pf1 = fork()) == 0)
		{
			
			pid_pere = getppid();
			if(pf2 = fork() == 0){
				kill(pid_pere,SIGUSR1);
				exit(0);

			}else{
				sigfillset(&sig_proc);
				sigdelset(&sig_proc,SIGCHLD);
				sigsuspend(&sig_proc);
				kill(pid_pere,SIGUSR2);
				wait(NULL);
				exit(0);
			}
			
			
		}
		else {
    
			sigfillset(&sig_proc);
			sigdelset(&sig_proc,SIGUSR1);
			sigsuspend(&sig_proc);
			printf("processus p3 cree\n");
			sigfillset(&sig_proc);
			sigdelset(&sig_proc,SIGUSR2);
			sigsuspend(&sig_proc);
			printf("processus p3 termine\n");
			sigfillset(&sig_proc);
			sigdelset(&sig_proc,SIGCHLD);
			sigsuspend(&sig_proc);
			printf("processus p2 termine\n");

		}
		
 
	return 0;
}

