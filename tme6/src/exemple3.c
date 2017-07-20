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
int *sp;
int tampon;

int main (int argc,char*argv[]){
	
	int fd;
	int size;
	if((fd = shm_open("monshm",O_RDWR|O_CREAT,0600))==-1){
		perror("shm_open");
		exit(1);	
	}
	if(ftruncate(fd,sizeof(int))==-1){
		perror("ftruncate");
		exit(1);	
	}
	if((sp=mmap(NULL,sizeof(int),PROT_READ|PROT_WRITE,MAP_SHARED,fd,0))==MAP_FAILED){
		perror("mmap");
		exit(1);	
	}
	*sp =10;
	if((size = read(fd,&tampon,sizeof(int))) <=0){
        perror("read error");
        return errno;
    }
	printf("tampon%d\n",tampon);
    return 0;
	
}
