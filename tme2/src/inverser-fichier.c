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

#define SIZE_TAMPON 10

int tampon[SIZE_TAMPON];
int status;

int main(int argc, char* argv[])
{
        
        int i=0 ;
        char c;  
        int fd1;
        int fd2;
		int p;
		if((fd1 = open(argv[1],O_RDONLY,0600))==-1){
            perror("open error");
            return errno;
        }
		if((fd2 = open(argv[2],O_WRONLY|O_CREAT,0600))==-1){
            perror("open error");
            return errno;
        }
		while(c != EOF){
			i++;
			if((lseek(fd1,-2-i,SEEK_END))==-1){
					perror("lseek error");
                    return errno;
			}
	    	if((p =read(fd1,&c,1))<=0){
		        perror("read errors");
		        exit(1);
		    }
	    	if((write(fd2,&c,1))== -1){
                 perror("write error");
                 return errno;
            }
			
		}
		
		  
		return 0;

}


