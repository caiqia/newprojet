#define _POSIX_SOURCE 1
#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#define N 10

pthread_mutex_t mutex_sig = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_cont = PTHREAD_COND_INITIALIZER;

int sig_cont;

void * thread_sig(void *arg)
{
	sigset_t ens;
	int sig;
	sigemptyset(&ens);
	sigaddset(&ens,SIGINT);
	while(1){
		sigwait(&ens,&sig);
		pthread_mutex_lock(&mutex_sig);
		sig_cont++;
		pthread_cond_signal(&cond_cont);
		if(sig_cont == 5){
			pthread_mutex_unlock(&mutex_sig);
			pthread_exit((void*)0);		
		}
	pthread_mutex_unlock(&mutex_sig);
	}
}

void * thread_cont(void *arg)
{
	sigset_t ens;
	sigfillset(&ens);
	pthread_sigmask(SIG_SETMASK,&ens,NULL);
	while(1){
		pthread_mutex_lock(&mutex_sig);
		pthread_cond_wait(&cond_cont,&mutex_sig);
		printf("cont:%d\n",sig_cont);
		if(sig_cont == 5){
			pthread_mutex_unlock(&mutex_sig);
			pthread_exit((void*)0);		
		}
	pthread_mutex_unlock(&mutex_sig);
	}
}

int main(int argc, char**argv)
{
	pthread_t tid_sig;
	pthread_t tid_cont;
	sigset_t ens;
	sigfillset(&ens);
	pthread_sigmask(SIG_SETMASK,&ens,NULL);
	if((pthread_create(&tid_cont,NULL,thread_cont,NULL)!=0)||(pthread_create(&tid_sig,NULL,thread_sig,NULL)!=0)){
		perror("pthread_affich \n");
		exit(1);	
	}
	if(pthread_detach(tid_sig)!=0){
		printf("pthread_detach\n");
		exit(1);	
	}
	if(pthread_join(tid_cont,NULL)!=0){
		printf("pthread_join\n");
		exit(1);	
	}
	printf("fin\n");
    return 0;
}
