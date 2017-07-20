#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <semaphore.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include "producteur_consommateur.h"
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <signal.h>

#define MAX_SIZE_STACK 10

struct shm{
	sem_t sem_prod;
	sem_t sem_cons;
	sem_t mutex;
	char pile[MAX_SIZE_STACK];
	int cpt;
};

struct shm *pt;
char var;

int push(char c){

	sem_wait(&(pt->sem_prod));
	sem_wait(&(pt->mutex));
	pt->pile[pt->cpt] = c;
	pt->cpt++;
	
	sem_post(&(pt->mutex));
	sem_post(&(pt->sem_cons));
	return 0;
}

int pop(char var){

	sem_wait(&(pt->sem_cons));
	sem_wait(&(pt->mutex));
	var = pt->pile[pt->cpt] ;
	pt->cpt--;
	//printf("%c\n",var);
	sem_post(&(pt->mutex));
	sem_post(&(pt->sem_prod));
	return 0;
}


int main (int argc, char* argv []) {
	int shm_id;
	
	shm_id = shm_open("/pro_con",O_RDWR|O_CREAT,0666);
	ftruncate(shm_id,sizeof(struct shm));
	pt = mmap(NULL,sizeof(struct shm),PROT_READ|PROT_WRITE,MAP_SHARED,shm_id,0);
	sem_init(&(pt->sem_prod),1,MAX_SIZE_STACK);
	sem_init(&(pt->sem_cons),1,0);
	sem_init(&(pt->mutex),1,1);
	pt->cpt = 0;

	push('a');
	push('b');
	push('c');
	pop(&var);
	printf("%c\n",var);

	return 0;
}



