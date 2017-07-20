#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <semaphore.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <signal.h>



int main (int argc, char* argv []) {

	int nb_pcs = atoi(argv[1]);
	int i;
	int k;
	int shm_id;
	sem_t s [nb_pcs-1];
	shm_id = shm_open("/shm_aff",O_RDWR|O_CREAT,0666);
	ftruncate(shm_id,sizeof(s));
	s = mmap(NULL,sizeof(s),PROT_READ|PROT_WRITE,MAP_SHARED,shm_id,0);
	for(i=0;i<nb_pcs;i++){
		sem_init(&s[i],1,0);	
	}
	
	for(k=0;k<nb_pcs;k++){
		if(fork()==0){
			sleep(nb_pcs%(k+1));
			if(k==0){
				sem_post(&s[0]);				
			}
			if(k==nb_pcs-1){
				sem_wait(&s[nb_pcs-2]);			
			}else{
				sem_wait(&s[k-1]);
				sem_post(&s[k]);
			}
			printf("identifiant %d , pid %d\n",k+1,getpid());		
		}
	}
	for(k=0;k<nb_pcs;k++){
		wait(NULL);
	}

	return 0;
}



