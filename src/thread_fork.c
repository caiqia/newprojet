#define _POSIX_SOURCE 1
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#define N 10

pthread_mutex_t mutex_sig = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_cont = PTHREAD_COND_INITIALIZER;


pthread_t tid[2];
char *nom[2] = {"T1","T2"};

void *func_thread(void *arg){

	printf("thread %s tid: %d, pid %d avant fork\n",arg,(int)pthread_self(),getpid());
	if(pthread_self() == tid[0]){
		if(fork()==0)
			printf("fils:%s tid:%d\n",arg,(int)pthread_self(),getpid());
		else
			printf("pere:%s tid %d\n",arg,(int)pthread_self(),getpid());
	}else
		printf("fin %s tid: %d,pid %d\n",arg,(int)pthread_self(),getpid());
	pthread_exit((void*)0);	
}


int main(int argc, char**argv)
{	
	int i;
	for(i=0;i<2;i++){
		if(pthread_create(&tid[i],NULL,func_thread,nom[i])!=0){
		perror("pthread_affich \n");
		exit(1);	
		}
	}

	for(i=0;i<2;i++){
		if(pthread_join(tid[i],NULL)!=0){
		printf("pthread_join\n");
		exit(1);	
		}
	
	}
	
    return 0;
}
