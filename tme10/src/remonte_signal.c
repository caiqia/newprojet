#define _XOPEN_SOURCE 700
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sched.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <signal.h>

void interrupt_signal(int signo, siginfo_t *si, void* context){
	
}

int main (int argc, char* argv []) {

	siginfo_t mysig;
	union sigval val;
	sigset_t block_mask;
	struct sigaction action;
	action.sa_sigaction = interrupt_signal;
	action.sa_flags = SA_SIGINFO;
	sigfillset(&block_mask);
	action.sa_mask = block_mask;
	sigaction(SIGRTMIN,&action,0);
	sigprocmask(SIG_SETMASK,&block_mask,0);

    pid_t pid_fils;
 	int numbre = 0;
	
	while ( numbre <= 2) 
	{
		if(fork())
		{	
			int d;
			sigemptyset(&block_mask);
			sigaddset(&block_mask,SIGRTMIN);
			if((sigwaitinfo(&block_mask,&mysig)) == -1){
				perror("sigwait"); exit(1);		
			}
			if(mysig.si_value.sival_int > 1){
					printf("ok\n");		
			}
			printf("receive %d\n",mysig.si_value);
			fflush(stdout);
			exit(0);
		}else {
			val.sival_int = (int)(rand () /(((double) RAND_MAX +1) /100));
			printf("send %d\n",val.sival_int);
			if((sigqueue(getppid(),SIGRTMIN,val))==-1){
				perror("sigqueue");exit(1);		
			}
			if(numbre==2){
				exit(0);			
				}
			
		}	
		numbre++;
	}
	
	return 0;
}



