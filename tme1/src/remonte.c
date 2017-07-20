#define _XOPEN_SOURCE 700
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

        int p;
        int n;
		n = argc;
        int i = 0;
        pid_t pid_fils;
        
        int fd1;
        int somme = 0;
		int somme2 = 0;
		int pt_ind;	

        if((fd1 = open(argv[2],O_RDWR|O_CREAT,0600))==-1){
            perror("open error");
            return errno;
        }
        while(i<n)
        {
            if((pid_fils = fork())== -1){
                perror("fork error");
                return errno;
            }
            if(pid_fils == 0){
				 pt_ind =(10*(float)rand()/ RAND_MAX);
				printf("aleatoire %d\n",pt_ind);
                if((write(fd1,pt_ind,sizeof(int)))== -1){
                    perror("write error");
                    return errno;
                }
                exit(pt_ind);
            }else{
                waitpid(pid_fils, &status,0);
				if(WIFEXITED(status)){
				somme += WEXITSTATUS(status);
				}
				
            }
			i++;

        }
		if((lseek(fd1,0,SEEK_SET))==-1){
					perror("lseek error");
                    return errno;
				}
        if( p =read(fd1,tampon,sizeof(tampon))<=0){
            perror("read errors");
            exit(1);
        }
     
        printf("somme egale: %d \n",somme);
	
    return 0;

}
