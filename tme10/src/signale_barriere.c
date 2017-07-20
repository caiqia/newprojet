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

int number ;
pid_t table[10];

void interrupt_signal(int signo, siginfo_t *si, void* context){
	
}
 void wait_barrier(int N){
		int i;
		int mysig;
		union sigval val;
		sigset_t block_mask;
		struct sigaction action;
		action.sa_sigaction = interrupt_signal;
		action.sa_flags = SA_SIGINFO;
		sigfillset(&block_mask);
		action.sa_mask = block_mask;
		sigaction(SIGRTMIN,&action,0);
		sigprocmask(SIG_SETMASK,&block_mask,0);

		if(N < number-1){	
				sigemptyset(&block_mask);
				sigaddset(&block_mask,SIGRTMIN);
				if((sigwait(&block_mask,&mysig)) == -1){
					perror("sigwait"); exit(1);		
				}
			}			
			else{
				table[N] = getpid();		
				for(i=0;i<=N;i++){			
					if((sigqueue(table[i],SIGRTMIN,val))==-1){
						perror("sigqueue");exit(1);		
					}
				}
			}
				
}

void process (int NB_PCS) {
   printf ("avant barrière\n");
	
   wait_barrier (NB_PCS);
		
   printf ("après barrière\n");	
	exit(0);
}

int main (int argc, char* argv []) {		
	int nb_pcs = atoi(argv[1]);
	number  = nb_pcs;
	int k;
	pid_t pid_fils;	
	for(k=0;k<nb_pcs;k++){
		if( (pid_fils = fork())==0){	
			process(k);
		}
		else{
			table[k] = pid_fils;	
		}
	}
	for(k=0;k<nb_pcs;k++){
		wait(NULL);		
	}
	return 0;
}
	



