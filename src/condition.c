#define _POSIX_SOURCE 1
#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define N 10

pthread_mutex_t mutex_fin = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_fin = PTHREAD_COND_INITIALIZER;

int flag=0;

void *func_thread(void *arg)
{
	pthread_mutex_lock(&mutex_fin);
	while(!flag){
		pthread_cond_wait(&cond_fin,&mutex_fin);
	}
	pthread_mutex_unlock(&mutex_fin);
	pthread_exit((void*)0);
}

int main(int argc, char**argv)
{
	pthread_t tid1;
	pthread_t tid2;

	pthread_mutex_lock(&mutex_fin);
	if((pthread_create(&tid1,NULL,func_thread,NULL)!=0)||(pthread_create(&tid2,NULL,func_thread,NULL)!=0)){
		perror("pthread_affich \n");
		exit(1);	
	}
	pthread_mutex_lock(&mutex_fin);
	flag=1;
	pthread_cond_broadcast(&cond_fin);
	pthread_mutex_unlock(&mutex_fin);
	
	pthread_join(tid1,NULL);
	pthread_join(tid2,NULL);

    return 0;
}
