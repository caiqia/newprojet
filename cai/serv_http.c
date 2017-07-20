#define _XOPEN_SOURCE 700


#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <time.h>
#include <pthread.h>

pthread_t tid;
int entetes(FILE* fich)
{
  char entete[200];
  int flag =  1;

  while (1) {
    	if((fgets(entete, sizeof(entete), fich))<=0){
				printf("%s\n","fgets entete");
	  			fclose(fich);
	  			pthread_exit((void*)0);
		}
    
    if (entete[0]=='\n' ||entete[0]=='\r') break;

    if (!strncasecmp(entete, "Connection:", 11) || strstr(entete, "close")){
		flag = 0;
	}
      
  }
  
  return flag;
}

char* type_fichier(char* chemin)
{
  int len = strlen(chemin);
  if (!strcasecmp(chemin+len-5, ".html") ||!strcasecmp(chemin+len-4, ".htm")) return "text/html";
  if (!strcasecmp(chemin+len-4, ".css")) return "text/css";
  if (!strcasecmp(chemin+len-4, ".png")) return "image/png";
  if (!strcasecmp(chemin+len-4, ".gif")) return "image/gif";
  if (!strcasecmp(chemin+len-5, ".jpeg") ||!strcasecmp(chemin+len-4, ".jpg")) return "image/jpeg";
  return "text/ascii";
}

void existpas(FILE* fich, char* chemin)
{
  	fprintf(fich, "HTTP/1.1 404 Not found\r\n");
 
  	printf("%s\n","404 not found");
  	fclose(fich);
  	pthread_exit((void*)0);
}

void *test1(void *scom)
{
	int i,j,k;	
	int flag = 1;
	i = *((int*)(scom));
	
	char un[100];
	char deux[100];
	char chemin[100];
	FILE* fich ;
	if((fich = fdopen(i, "r+"))==0){
		printf("fdopen\n");exit(1);
	}
	
	setlinebuf(fich);
	
	
 	 while (flag) {
	  	if((fgets(un, sizeof(un), fich))<=0){
			printf("%s\n","fgets un");
  			fclose(fich);
  			pthread_exit((void*)0);
		}
	
	  if ((strncmp(un, "GET", 3))!=0){
	  		printf("%s\n","GET error");
  			fclose(fich);
  			pthread_exit((void*)0);
	  } 
		
	  for (j=3; un[j]==' '; j++);
	  for (;un[j] && un[j]!='/'; j++);
	  if (un[j] =='/') j += 5 ;
      for (k=0; un[j] && un[j]!=' ' && k<sizeof(chemin)-1; k++, j++){
		chemin[k] = un[j];
	  }
	  chemin[k] = 0;
	  for (; un[j]==' '; j++);
	  if ((strncmp(un+j, "HTTP/1.1", 8))!=0){
			printf("%s\n","HTTP error");
	  		fclose(fich);
	  		pthread_exit((void*)0);
	  }
	
	  /*if((fgets(deux, sizeof(deux), fich))<=0){
			printf("%s\n","fgets deux");
  			fclose(fich);
  			pthread_exit((void*)0);
		}
	  if ((strcmp(deux, "Host: 127.0.0.1"))!=0){
	  		printf("%s\n","ligne deux error");
  			fclose(fich);
  			pthread_exit((void*)0);
	  } */
		
	  flag = entetes(fich);			  
	  printf("URL: %s\n",chemin);

		  char context[5000];
		  struct stat s;
		

		int fd;
	  if((fd = open(chemin, O_RDONLY))==-1){
			existpas(fich, chemin); 
			return 0;
	  }
	
      if (fstat(fd, &s)==-1 || !S_ISREG(s.st_mode) || !(s.st_mode & S_IROTH)){
			 close(fd); 
			existpas(fich, chemin); 
			return 0; 
		}
		 if((fprintf(fich, "Content-type: %s\r\n", type_fichier(chemin)))<=0){
				printf("fprintf\n");exit(1);
		}
		fflush(fich);
		if((fprintf(fich, "%s\r\n","HTTP/1.1 200 OK"))<=0){
			printf("fprintf\n");exit(1);
		}
		fflush(fich);
	  if((fprintf(fich, "Connection: %s\r\n", flag ? "keep-alive" : "close"))<=0){
			printf("fprintf\n");exit(1);
		}
		fflush(fich);
	  if((fprintf(fich, "Content-length: %li\r\n", (long)s.st_size))<=0){
				printf("fprintf\n");exit(1);
		}
		fflush(fich);
	 
	fflush(fich);
	  if((fprintf(fich, "\r\n"))<=0){
			printf("fprintf\n");exit(1);
		}
	
	  while (1) {
		 int csize;
		int m;
		int cpt;
		if((csize = read(fd,context,sizeof(context))) <=0){
		    perror("read error");
		    return errno;
		}
		printf("%s\n",context);
		
		if (csize==0) break;
		if (csize<0) {
		  
			  close(fd);
			printf("%s\n","read context");
		  	fclose(fich);
		  	pthread_exit((void*)0);
		}
		 fflush(fich);
		/*for (m=0; m<csize; ) {
			if((cpt = fwrite(context+m, 1, csize-m, flog))<=0){
				if (errno==EINTR) continue;
				close(fd);
				printf("%s\n","write context");
		  		fclose(fich);
		  		pthread_exit((void*)0);
			}
		  m += cpt;
		}
			*/
		for (m=0; m<csize; ) {
			if((cpt = fwrite(context+m, 1, csize-m, fich))<=0){
				if (errno==EINTR) continue;
				close(fd);
				printf("%s\n","write context");
		  		fclose(fich);
		  		pthread_exit((void*)0);
			}
		  m += cpt;
		}
		
       }

        close(fd);
	}
	printf("%s\n","ok");
	 fclose(fich);
	return 0;
  }
	

int main(int argc, char *argv[])
{
	 signal(SIGPIPE, SIG_IGN);
	struct sockaddr_in sin; 
	struct sockaddr_in exp; 
   	char nomfich[10];
	int on = 1;
	char host[64];

    int sc;
	int scom;
	int fd;
	int fromlen = sizeof(exp);
	signal(SIGPIPE, SIG_IGN);

	if((sc = socket(AF_INET,SOCK_STREAM,0))<0){
		perror("socket");exit(1);	
	}
	if((setsockopt(sc,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on)))==-1){
		perror("setsockopt");exit(1);	
	}
	
	memset(&sin,0,sizeof(sin));
	sin.sin_port = htons(8080);//atoi(argv[1]));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = htonl(INADDR_ANY); 
	if(bind(sc,(struct sockaddr *)&sin,sizeof(sin))<0){
		perror("bind");exit(1);	
	}
	listen(sc,atoi(argv[1]));

	while(1){
		if((scom=accept(sc,(struct sockaddr *)&exp,&fromlen))==-1){
			perror("accept"); exit(2);				
		}
		
		int* pt_ind;
	 	pt_ind =(int*) malloc(sizeof (int));
		*pt_ind = scom;
		if(pthread_create(&tid,NULL,test1,pt_ind)!=0){
			perror("pthread_creat \n");
			exit(1);	
		}
		if(( pthread_detach(tid))!=0){
			perror("pthread_detach \n");
			exit(1);	
		}
	
	}
		
    return 0;
}
