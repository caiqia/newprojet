#define _POSIX_SOURCE 1
#define _REENTRANT
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <pthread.h>
#include <unistd.h>
#include "producteur_consommateur.h"

#define pilesize 100
#define chainesize 10


pthread_mutex_t mutex_fin = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_fin = PTHREAD_COND_INITIALIZER;

pthread_t  tid_p;
pthread_t  tid_c;

char pile[pilesize];
int stack_size = 0;
int numbrethread = 0;

void push(char c){
	pthread_mutex_lock(&mutex_fin);
		if(stack_size < pilesize){
			pile[stack_size] = c;
			stack_size++;
		}
	pthread_mutex_unlock(&mutex_fin);
	
}

char pop(){
	pthread_mutex_lock(&mutex_fin);
		if(stack_size > 0){
			stack_size--;
			return pile[stack_size];
		}
	pthread_mutex_unlock(&mutex_fin);
	
}
void *test(void *path)
{

	printf("argument recu %s, tid: %d \n",path,(int)pthread_self());
	
	pthread_exit((void*)0);	
}

int main(int argc, char**argv)
{
	int i;
	char* status;
	int k;

	if(pthread_create(&tid_p,NULL,test,"ygyu")!=0){
			perror("pthread_creat \n");
			exit(1);	
	}
	if(pthread_create(&tid_c,NULL,test,"gjk")!=0){
			perror("pthread_creat \n");
			exit(1);	
	}	
	printf("tid_p %d tid_c %d\n",tid_p,tid_c);
	if(pthread_join(tid_p,NULL)!=0){
			perror("pthread_join \n");
			exit(1);	
	}
	if(pthread_join(tid_c,NULL)!=0){
			perror("pthread_join \n");
			exit(1);	
	}
    return 0;
}



