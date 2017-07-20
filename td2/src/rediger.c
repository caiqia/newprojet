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

void rediriger_stdout(int fd){
	dup2(fd,STDOUT_FILENO);
		
}

void restaurer_stdout(){
	int d = open("/dev/tty",O_WRONLY);
	dup2(d,STDOUT_FILENO);
	
}

int main(int argc, char* argv[])
{
    int fd1;
	int n;
	int size;
	int i = 0;
		if((fd1 = open(argv[1],O_RDWR|O_CREAT,0600))==-1){
		    perror("open error");
		     return errno;
		}
		rediriger_stdout(fd1);
		printf("bonjour\n");
		printf("toto\n");
		restaurer_stdout();
		printf("tu es bien\n");
 				

return 0;

}



