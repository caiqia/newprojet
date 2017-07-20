#define _POSIX_SOURCE 1
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <time.h>
#include <pthread.h>

void sig_hand(int sig){

}

void *test(void*arg){
	int i,j;
	for(j=0;j<3;j++){
		//for(i=0;i<1000;i++);
		printf("thread %d\n",(int)pthread_self());
		//sched_yield();
	}
return 0;	
}

int main(int argc, char *argv[])
{
	pthread_t tid[3];
	int i;
	for(i=0;i<3;i++){
	
		if((pthread_create(&tid[i],NULL,test,NULL))!=0){
		 perror("creat\n");exit(1);
		}	
	}
	sleep(3);
	printf("fin");
return(0);

}
