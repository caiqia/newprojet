
#include <mqueue.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>

#define MSG_SIZE  128

static struct mq_attr attr;

int main (int argc,char*argv[]){
	mqd_t mqdes;
	char buf[100];
	unsigned int prio;
	if((mqdes = mq_open("/file1",O_RDWR|O_CREAT,0666,NULL))==-1){
		perror("mq_open");
		exit(1);	
	}
	if(fork()==0){
		if((mq_receive(mqdes,buf,(size_t)attr.mq_msgsize,&prio))==-1){
			perror("mq_rec");
			exit(1);		
		}
		printf("message: %s prio: %d. \n",buf,prio);	
	}else{
		if((mq_send(mqdes,"abcd",4,0))==-1){
			perror("mq_send");
			exit(1);		
		}
		wait(NULL);
	}
	mq_close(mqdes);
	mq_unlink("/file1");
	return 0;

}
