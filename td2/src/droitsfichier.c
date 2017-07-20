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
    
    struct stat stat_info;
	
    if((stat(argv[2], &stat_info))== -1){
       perror("stat error");
       return errno;
	}
	if(S_ISDIR(stat_info.st_mode)){
		perror("c'est repertoire");
		  return 0;
	}

	if((!strcmp(argv[1],"E"))||(!strcmp(argv[1],"e"))){
		if((unlink(argv[2]))==-1){
			perror("unlink error");
			return errno;
		}
	}
	if((!strcmp(argv[1],"R"))||(!strcmp(argv[1],"r"))){        
		if((rename(argv[2],argv[3]))==-1){
			perror("rename error");
			return errno;
		}
	}
	if((!strcmp(argv[1],"C"))||(!strcmp(argv[1],"c"))){
			if((!strcmp(argv[3],"R"))||(!strcmp(argv[3],"r"))){
				if((chmod(argv[2],S_IRUSR))==-1){
					perror("chmod error");
					return errno;
				}
			}
			if((!strcmp(argv[3],"W"))||(!strcmp(argv[3],"w"))){
				if((chmod(argv[2],S_IWUSR))==-1){
					perror("chmod error");
					return errno;
				}
			}

	}

return 0;
}
