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


struct shm{
	int cpt;
	sem_t smutex;
	sem_t barrier;
};

struct shm*pt;

struct shm* init_barrier(){
	int shm_id;
	struct shm *p;
	shm_id = shm_open("/shm_bar",O_RDWR|O_CREAT,0666);
	ftruncate(shm_id,sizeof(struct shm));
	p = mmap(NULL,sizeof(struct shm),PROT_READ|PROT_WRITE,MAP_SHARED,shm_id,0);
	sem_init(&(p->smutex),1,1);
	sem_init(&(p->barrier),1,0);
	return p;
}
 void wait_barrier(int N){
		int i;
		sem_wait(&(pt->smutex));
		pt->cpt++;
		if(pt->cpt < N){
			sem_post(&(pt->smutex));	
			sem_wait(&(pt->barrier));
				
		}else{
			for(i=0;i< N-1;i++){
				sem_post(&(pt->barrier));
				pt->cpt=0;		
			}
		sem_post(&(pt->smutex));	
		}
}


void process (int NB_PCS) {
   printf ("avant barrière\n");
   wait_barrier (NB_PCS);
   printf ("après barrière\n");
	exit(0);
}

int main (int argc, char* argv []) {

	pt = init_barrier();
	int nb_pcs = atoi(argv[1]);
	pt->cpt = 0;
	int number=0;
	pid_t pid_fils;
	int k;

	for(k=0;k<nb_pcs;k++){
		if(fork()==0){
			process(nb_pcs);
			
		}
	}
	for(k=0;k<nb_pcs;k++){
		wait(NULL);
		
	}
	shm_unlink("/shm_bar");
	return 0;
}



