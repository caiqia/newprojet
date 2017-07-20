#define _POSIX_SOURCE 1
#define _REENTRANT
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <pthread.h>


#define chainesize 10

pthread_mutex_t mutex_fin = PTHREAD_MUTEX_INITIALIZER;

pthread_t  tid[chainesize];


int counter = 2;
int numbre = 0;



void *test(void *tab)
{
	pthread_mutex_lock(&mutex_fin);
	
	while(counter <numbre){
		
		printf("tab%s\n",((char**)tab)[2]);
		int x = counter;
		counter++;
		pthread_mutex_unlock(&mutex_fin);  //quelqun autre utilise source
		if(upper(((char**)tab)[x]) != 0){	
			pthread_exit(((char**)tab)[x]);
		}else{
			pthread_exit((void*)0);	
		}
		pthread_mutex_lock(&mutex_fin);  //retour au boucle
	}
	pthread_mutex_unlock(&mutex_fin);	
}

int main(int argc, char**argv)
{
	int i;
	char* status;
	int k;

    numbrethread = atoi(argv[1]);

    numbre = argc ;

	for(i=0;i< argv[1];i++){	
		if(pthread_create(&(tid[i]),NULL,test,argv)!=0){
			perror("pthread_creat \n");
			exit(1);	
		}	
	}
	for(i=0;i< argv[1];i++){
		if(pthread_join(tid[i],(void*)&status)!=0){
		perror("pthread_join \n");
		exit(1);	
		}else{
			printf("nom de fichier pas traite %s \n",status);		
		}
	}
    return 0;
}


int upper(char *path)
{
    FILE* fp1, *fp2;
    int c = 1;

    fp1= fopen (path, "r");
    fp2= fopen (path, "r+");
    if ((fp1== NULL) || (fp2== NULL)) {
        perror (path);
	return errno;
    }

    while (c !=EOF) {
        c=fgetc(fp1);
        if (c!=EOF)
        fputc(toupper(c),fp2);
    }

    fclose (fp1);
    fclose (fp2);

    return EXIT_SUCCESS;
}
