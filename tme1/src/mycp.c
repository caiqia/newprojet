
#define _POSIX_SOURCE 1
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>

#define SIZE_TAMPON 100

char tampon[SIZE_TAMPON];
int status;

int main(int argc, char* argv[])
{
    int fd1,fd2,fd3;
    int size,i;
	if((fd3 = open("dup.txt",O_WRONLY,0600))==-1){
        perror("open1 error");
        return errno;
    }
    if((fd1 = open(argv[1],O_RDWR,0600))==-1){
        perror("open1 error");
        return errno;
    }
    if((fd2 = open(argv[2],O_RDWR|O_CREAT,0600)) ==-1){
        perror("open2 error");
        return errno;
    }

    if((size = read(fd1,tampon,SIZE_TAMPON)) <=0){
        perror("read error");
        return errno;
    }
	dup2(fd3,STDERR_FILENO);	
	
    if((write(fd2,tampon,size))== -1){
        perror("write error");
        return errno;
    }
	
    return 0;
}
