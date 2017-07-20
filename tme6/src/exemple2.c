#include <sys/mman.h>
#include <mqueue.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>


#define MSG_SIZE  128
int nr;

static void tfunc(union sigval sv){
	char buf[100];
	mqd_t mqdes = *((mqd_t*)sv.sival_ptr);
	if((nr=mq_receive(mqdes,buf,100,NULL))==-1){
		perror("mq_receive");
		exit(1);	
	}
	printf("lu %ld\n",(long)nr);
	exit(0);	
}

int main (int argc,char*argv[]){
	
	mqd_t mqdes;
	struct sigevent not;
	unsigned int prio;
	mqdes = mq_open(argv[1],O_RDONLY);
	
	if(mqdes == (mqd_t)-1){
		perror("mq_open");
		exit(1);	
	}
	not.sigev_notify = SIGEV_THREAD;
	not.sigev_notify_function = tfunc;
	not.sigev_notify_attributes = NULL;
	not.sigev_value.sival_ptr = &mqdes;
	if(mq_notify(mqdes,&not)==-1){
		perror("mq_notify");
		exit(1);	
	}
	pause();
}
