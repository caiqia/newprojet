#define _POSIX_SOURCE 1
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>
#define SIZE_TAMPON 100


char tampon[SIZE_TAMPON];
int status;


int main(int argc, char* argv[])
{
    int fd1,fd2;
    struct stat stat_info1;
	struct stat stat_info2;
    if((lstat(argv[1], &stat_info1))== -1){
       perror("stat error");
       return errno;
	}
	if((lstat(argv[2], &stat_info2))== -1){
		   perror("stat error");
       return errno;
		}
	if(stat_info1.st_ino == stat_info2.st_ino){
	printf("c'est la meme fichier");
	
	}else{
		printf("c'est pas la meme fichier");	
	}
	

}
