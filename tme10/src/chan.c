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

#define n 3

pid_t table[n];
	int status;

int valeuraleatoire = 0;

int main (int argc, char* argv []) {

	int i = 0 ;

	int retour_fils;

    pid_t pid_fils;
	int k;
	
	for(k=0;k<n;k++){
		if( (pid_fils = fork())==0){		
			if(k < n-1){
				printf("N %d\n",k);		
				
				printf("tablett %d\n",table[k]);
				//sigemptyset(&block_mask);
				//sigaddset(&block_mask,SIGRTMIN);
				//if((sigwait(&block_mask,&mysig)) == -1){
				//	perror("sigwait"); exit(1);		
				//}
			}
			
			else{
				printf("N2 %d\n",k);		
				for(i=0;i<k;i++){
					printf("table2  %d\n",table[i]);				
					//if((sigqueue(table[i],SIGRTMIN,val))==-1){
						//perror("sigqueue");exit(1);		
					//}
				}
			}
			
	
		}
		else{
			table[k] = pid_fils;		
		}
	}
	for(k=0;k<n;k++){
		wait(NULL);
		
	}

 
	return 0;
}



