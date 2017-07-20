#define _XOPEN_SOURCE 700
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char* argv []) {
	int p;
	int i = 1;
	int n = 3;
	do{
		p = nfork(i);
		if(p !=0){
			printf("%d\n", p);			
			}
	}while((p==0)&&(++i <=n));
	printf("fin\n");
	return EXIT_SUCCESS;
}

int nfork (int n)
{

	int numbre = 0;
    	pid_t pid;
	while ( numbre < n) 
	{
		if((pid = fork()) == 0)
		{
			 return 0;
		}
		if(pid == -1 ) {
			return numbre == 0 ? -1 : numbre;
		} else {
        	numbre++;
		}
 
	}
	return numbre;
}



