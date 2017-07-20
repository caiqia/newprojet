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

	char* nom = argv[1];
	char* chaine = argv[2];
	char* tampon;
	int fd1,fd2,size;
	if((fd1 = open(nom,O_WRONLY|O_CREAT,0600))==-1){
        perror("open1 error");
        return errno;
    }
	 struct aiocb a;
	 struct aiocb  *aio_list[1] = {&a};
	a.aio_fildes = fd1;
	a.aio_buf = chaine;
	a.aio_nbytes = strlen(chaine);
	a.aio_offset = 0;
	a.aio_reqprio = 0;
	a.aio_sigevent.sigev_notify =0;
	a.aio_sigevent.sigev_signo = 0;

	if((aio_write(&a))==-1){
		perror("aiowrite\n");exit(1);
	}
	
	if((aio_suspend(aio_list,1,0))==-1){
		perror("aiosusp\n"); exit(1);	
	}
	printf("gdgd\n");
	if((fd2 = open(nom,O_RDONLY|O_CREAT,0600))==-1){
        perror("open1 error");
        return errno;
    }

	
	if((aio_read(&a))==-1){
		perror("aioread\n");exit(1);
	}
	tampon = a.aio_buf;
	printf("tampon %s\n",tampon);
	return 0;
}



