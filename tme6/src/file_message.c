#include <mqueue.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>


int main (int argc, char* argv []) {
	
	mqd_t mqdes;
	struct mq_attr attr;
	char* buf;
	unsigned int prio;
	int i;
	int val;
	pid_t pid_fils;
	if((mqdes = mq_open("/file1",O_RDWR|O_CREAT,0666,NULL))==-1){
		perror("mq_open");
		exit(1);	
	}
	if(mq_getattr(mqdes,&attr)==-1){
		perror("getattr");	
	}
	buf = malloc(attr.mq_msgsize);
	//int nb_pcs = atoi(argv[1]);
  // for(i=0;i<nb_pcs;i++){
		if((pid_fils = fork()) == 0)
		{
			if((mq_receive(mqdes,buf,1000,&prio))==-1){
				perror("mqrec");exit(1);		
			}
			
		printf("message %d\n",buf);
		
		}
	else{
		if((mq_send(mqdes,"abcd",4,0))==-1){
				perror("mqsend"); exit(1);			
			}
		wait(NULL);
			
	}

	mq_close(mqdes);
	mq_unlink("/file1");
	return 0;
}
