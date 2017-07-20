#define _POSIX_SOURCE 1
#define _REENTRANT
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/stat.h>
#include <errno.h>
#include <pthread.h>
#include <sys/wait.h>
#include <fcntl.h>

#define S_BUF 100


char buffer[S_BUF];




int main(int argc, char**argv)
{
	int i;
 	int k;
    int fd_read;
	int fd_write;
	char message[100];
	if(mkfifo(argv[1],S_IRUSR|S_IWUSR)==-1){
		perror("mkfifo");
		exit(1);	
	}
	while(1){
		if((fd_read = open(argv[1],O_RDONLY))==-1){
			perror("open");	
			exit(3);		
		}
		if((k= read (fd_read,buffer,S_BUF))==-1){
			perror("read");
			exit(1);		
		}else{
										
			for(i=0;i<k;i++){
				message[i] = toupper(buffer[i]);
			}		
			message[k] = '\0';	
		}
		write(STDOUT_FILENO,message,k);
	}
	return 0;
}





