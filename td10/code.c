#define _XOPEN_SOURCE 700
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sched.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <aio.h>
#include <time.h>

void interrupt_signal(int signo, siginfo_t *si, void* context){
	
}

int main (int argc, char* argv []) {

	  static struct aiocb   aio[2];

  static struct aiocb  *aio_list[2] = {&aio[0],&aio[1]};

  int i, ret,fd1,fd2;

char* nom1 = argv[1];
char* nom2 = argv[2];
	char* chaine = argv[3];
if((fd1 = open(nom1,O_WRONLY|O_CREAT,0600))==-1){
        perror("open1 error");
        return errno;
    }
	if((fd2 = open(nom2,O_WRONLY|O_CREAT,0600))==-1){
        perror("open1 error");
        return errno;
    }
	aio[0].aio_fildes = fd1;
	aio[0].aio_buf = chaine;
	aio[0].aio_nbytes = strlen(chaine);
	aio[0].aio_offset = 0;
	//aio[0].aio_reqprio = 0;
	//aio[0].aio_sigevent.sigev_notify = SIGEV_SIGNAL;
	//aio[0].aio_sigevent.sigev_signo = SIGRTMIN; 
	aio[1].aio_fildes = fd2;
	aio[1].aio_buf = chaine;
	aio[1].aio_nbytes = strlen(chaine);
	aio[1].aio_offset = 0;
	//aio[1].aio_reqprio = 0;
	

/* Asynch-write */

if (aio_write(&aio[0]) == -1) return errno;

if (aio_write(&aio[1]) == -1) return errno;

 



 

/* do the wait operation using sleep() */

ret = aio_suspend(aio_list, 2, 0);
printf("frfwfe\n");
if (ret == -1) return errno;

	return 0;
}



