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
int counter = 0;

static void *affich(void *arg)
{
	pthread_mutex_lock(&mutex_fin);
	pthread_cond_wait(&cond_fin,&mutex_fin);
	pthread_mutex_unlock(&mutex_fin);
	printf("variable globale:%d\n",variable);
}

void *test(void *arg)
{
	int i;
	int*temp;
	temp = (int*)malloc(sizeof(i));
	i = *((int*)(arg));
	*temp = i*2;
	
	printf("argument recu %d, tid: %d\n",i,(int)pthread_self());
	
	pthread_mutex_lock(&mutex_fin);
	counter++;
	variable += (int) (10*((double)rand())/ RAND_MAX);
	if(counter == N){
		pthread_cond_signal(&cond_fin);
	}
	pthread_mutex_unlock(&mutex_fin);
	
	pthread_exit((void*)temp);
return NULL;
}

int main(int argc, char**argv)
{
	int i,*status;
	int* pt_ind;
	pthread_t tid2;

	if((pthread_create(&(tid[i]),NULL,affich,NULL))!=0){
		perror("pthread_affich \n");
		exit(1);	
		}
   
	
	for(i=0;i<N;i++){
		pt_ind = (int*)malloc(sizeof(i));
		*pt_ind = i;	
		if((pthread_create(&(tid[i]),NULL,test,(void*)pt_ind))!=0){
		perror("pthread_creat \n");
		exit(1);	
		}
	}
	for(i=0;i<N;i++){
		if((pthread_join(tid[i],(void**)&status))!=0){
		perror("pthread_join \n");
		exit(1);	
		}else{
			printf("thread %dfini avec status:%d\n",i,*status);		
		}
	}

    return 0;
}
