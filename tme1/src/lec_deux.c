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
////
        
		char* nom = argv[2];
		char* op = argv[1];
        int i = 0;
        pid_t pid_fils;
        
        int fd1;
        int somme = 0;
		int somme2 = 0;
		int* pt_ind;
		
        while(i<2)
        {
			
            if((pid_fils = fork ())== 0){
					
				if(i == 0){
					if((strcmp(op,"-p"))==0){
					
						lecture1(nom);
					}
						
					if((strcmp(op,"-c"))==0)
						lecture2(nom);
					printf("pid: %d\n",getpid());		
				}
				if(i == 1){
					if((strcmp(op,"-p"))==0)
						lecture1(nom);
					if((strcmp(op,"-c"))==0)
						lecture2(nom);
					printf("pid: %d\n",getpid());				
				}
				exit(0);
		           
		    }else{
					
		            wait(NULL);						
		        }
		i++;

		  } 
	
		return 0;

	}

int lecture2(char* nom){
		FILE*pt1;
		char c;
		if((pt1 = fopen(nom, "r")) == NULL){
			perror("fopen");
			exit(1);	
		}
	while( (c=fgetc(pt1)) != EOF)
		printf("%c\n",c);
			printf("nom %s\n",nom);
	return 0;
}

int lecture1(char* nom){
	
	int fd1;
	int p;
	char c ;

	if((fd1 = open(nom,O_RDONLY,0600))==-1){
		perror("open");
		exit(1);	
	}
	while(c != EOF){
		if( (p =read(fd1,&c,1))<=0){
            perror("read errors");
            exit(1);
        }else{
			printf("%c\n",c);
		}
	}
		  
return 0;
}
