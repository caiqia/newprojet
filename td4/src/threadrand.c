#define _POSIX_SOURCE 1
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define N 10

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_t  tid[N];
int variable = 0;

void *test(void *arg)
{
	int i;
	int*temp;
	int x;
	temp = (int*)malloc(sizeof(i));
	i = *((int*)(arg));
	printf("argument recu %d, tid: %d\n",i,(int)pthread_self());
	
	*temp = i*2;

	pthread_mutex_lock(&mutex);
	x = (int) (10*((double)rand())/ RAND_MAX);
	variable += x;
   	printf("x: %d\n",x);
	pthread_mutex_unlock(&mutex);
	pthread_exit((void*)temp);
return NULL;
}

int main(int argc, char**argv)
{
	int i,*status;
	int* pt_ind;

	
	for(i=0;i<N;i++){
		pt_ind =(int*) malloc(sizeof (int));
		*pt_ind = i;
		if(pthread_create(&(tid[i]),NULL,test,(void*)pt_ind)!=0){
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
	
	printf("variable globale:%d\n",variable);

    return 0;
}
