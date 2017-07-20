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

#define n 10

pid_t chaine[n];
	int status;

int valeuraleatoire = 0;

int main (int argc, char* argv []) {

	int i = 0 ;
	int numbre = 0;
	int retour_fils;

    pid_t pid_fils;

	
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
				
					valeuraleatoire = (int)(rand () /(((double) RAND_MAX +1) /100));
					printf("valeuraleatoire %d \n",valeuraleatoire);
					exit(valeuraleatoire);	
			}else{
				chaine[numbre] = getppid();					
			}
		}
		 else {
			waitpid(pid_fils,&status,0);
			if(WIFEXITED(status)){
				if(numbre == n){
					
						printf("status : %d \n", WEXITSTATUS(status));
						printf("pid : %d , pid de pere : %d , pas de fils  \n", getpid(),getppid());
						exit(WEXITSTATUS(status));
						}
			
				if(numbre == 0){
					
					
							printf("valeuraleatoire %d \n", WEXITSTATUS(status));
							printf("pid : %d , pas de pere , pid de fils : %d  \n", getpid(),pid_fils);		
							return 0;

				}else{				
						printf("pid : %d , pid de pere : %d , pid de fils : %d  \n", getpid(),getppid(),pid_fils);
						exit(WEXITSTATUS(status));	
					 }
				}
			}
			
		numbre++;
 
	}
	return 0;
}



