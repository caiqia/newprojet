#define _POSIX_SOURCE 1
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define N 10

pthread_mutex_t mutex_fin = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_fin = PTHREAD_COND_INITIALIZER;

pthread_t  tid[N];
int variable = 0;

void *test(void *arg)
{
	int i;
	pthread_mutex_lock(&mutex_fin);
	variable = (int) (10*((double)rand())/ RAND_MAX);
	printf("argument recu %d, tid: %d\n",(int*)arg,(int)pthread_self());
	printf("varaible aleatoire %d\n",variable);
	pthread_mutex_unlock(&mutex_fin);
	pthread_exit((void*)0);
return NULL;
}

int main(int argc, char**argv)
{
	int i;
	int* pt_ind;
	
	for(i=0;i<N;i++){
		pt_ind = (int*)malloc(sizeof(int));
		*pt_ind = i;
		if(pthread_create(&(tid[i]),NULL,test,(void*)pt_ind)!=0){
		perror("pthread_creat \n");
		exit(1);	
		}
		if(pthread_detach(tid[i])!=0){
				perror("pthread_detach \n");
				exit(1);					
		}
	
	}

    return 0;
}
