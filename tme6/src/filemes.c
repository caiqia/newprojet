#define _XOPEN_SOURCE 700
#include <sys/mman.h>
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

#define n 4

int status;

int tampon;
int valeuraleatoire = 0;


int main (int argc, char* argv []) {

	int somme = 0 ;
	int numbre = 0;
	int fd1;
	int fd2;
	int size;
	int i;
    pid_t pid_fils;
	int nb_pcs = atoi(argv[1]);
	int* sp;
	if((fd1 = shm_open("ccshm",O_RDWR|O_CREAT,0600))==-1){
			perror("shm_open");
			exit(1);	
		}
	if(ftruncate(fd1,100)==-1){
			perror("ftruncate");
			exit(1);	
	}
	if((sp=mmap(NULL,100,PROT_READ|PROT_WRITE,MAP_SHARED,fd1,0))==MAP_FAILED){
			perror("mmap");
			exit(1);	
		}	
	for(i=0;i<nb_pcs;i++){
		if((pid_fils = fork()) == 0)
		{
			valeuraleatoire = (int)(10*(float)rand () / RAND_MAX + i);
			sp[i] = valeuraleatoire;
			printf("valeuraleatoire %d \n",valeuraleatoire);
			exit(0);	
		
		}
	}
	for(i=0;i<nb_pcs;i++){
			wait(NULL);
			printf(" %d\n",sp[i]);
	}
	munmap(sp,100);
	shm_unlink("ccshm");
	return 0;
}



