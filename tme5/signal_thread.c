#define _POSIX_SOURCE 1
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#define N 10

pthread_mutex_t mutex_fin = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_cont = PTHREAD_COND_INITIALIZER;


pthread_t tid[N];
int counter;

void *func_thread(void *arg){
	
	pthread_mutex_lock(&mutex_fin);
	printf("counter %d, tid: %d\n",counter,(int)pthread_self());
	
	tid[counter] = pthread_self();
	counter++;

	if(counter < N){
		if(pthread_create(&tid[counter],NULL,func_thread,NULL)!=0){
			perror("pthread_creat \n");
			exit(1);	
		}else{
			sigset_t ens;
			int sig;
			sigemptyset(&ens);
			sigaddset(&ens,SIGUSR1);		
			sigwait(&ens,&sig);	
			if(pthread_kill(tid[counter],SIGUSR1)!=0){
				perror("pthread_kill \n");
				exit(1);
			}
			pthread_mutex_unlock(&mutex_fin);
			pthread_exit((void*)0);	
		}
	}else{
		if(pthread_kill(tid[0],SIGUSR1)!=0){
			perror("pthread_kill \n");
			exit(1);
		}
		pthread_mutex_unlock(&mutex_fin);
		pthread_exit((void*)0);		
	}	
		
}

int main(int argc, char**argv)
{	
	int i;
	sigset_t ens;
	sigfillset(&ens);
	pthread_sigmask(SIG_SETMASK,&ens,NULL);

	pthread_mutex_lock(&mutex_fin);
	tid[0] = pthread_self();
	counter = 1;	
	if(pthread_create(&tid[counter],NULL,func_thread,NULL)!=0){
	perror("pthread_affich \n");
	exit(1);	
	}
	pthread_mutex_unlock(&mutex_fin);

	int sig;
	sigemptyset(&ens);
	sigaddset(&ens,SIGINT);		
	sigwait(&ens,&sig);

	if(pthread_kill(tid[1],SIGUSR1)!=0){
		perror("pthread_kill \n");
		exit(1);
	}
	sigemptyset(&ens);
	sigaddset(&ens,SIGUSR1);
	sigwait(&ens,&sig);
	printf("Tous mes descendants sont créé\n");
	

	for(i=1;i<N;i++){
		if(pthread_join(tid[i],NULL)!=0){
		printf("pthread_join\n");
		exit(1);	
		}	
	}
	printf("Tous mes descendants se sont terminés\n");
    return 0;
}
