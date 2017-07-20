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

pthread_t  tid;


pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int execute(char* un,int fd,char* command){
			 sigset_t blockMask;
   			 struct sigaction saIgnore, saOrigQuit, saOrigInt, saDefault;
    		 pid_t pid_fils;
   			 int status;
    		if (command == NULL)
       	    return system(":") == 0;

				sigemptyset(&blockMask);
				sigaddset(&blockMask, SIGCHLD);
				pthread_sigmask(SIG_BLOCK, &blockMask,0);

				saIgnore.sa_handler = SIG_IGN;
				saIgnore.sa_flags = 0;
				sigemptyset(&saIgnore.sa_mask);
				sigaction(SIGINT, &saIgnore, 0);
				sigaction(SIGQUIT, &saIgnore,0);

				switch (pid_fils = fork()) {
				case -1: 
					status = -1;
					break;         
				case 0:
					saDefault.sa_handler = SIG_DFL;
					saDefault.sa_flags = 0;
					sigemptyset(&saDefault.sa_mask);
					sigaction(SIGINT, &saDefault,0);
					sigaction(SIGQUIT, &saDefault,0);

					execl("/bin/sh", "sh", "-c", command, (char *) 0);
					_exit(127);                   

				default:
					
					while (waitpid(pid_fils, &status, 0) != pid_fils) {
						if (errno != EINTR) {       
						    status = -1;                    
						}
						else{
							sleep(10);
							kill(pid_fils,SIGQUIT);
							if((dprintf(fd, "%s\r\n","HTTP/1.1 500 Internal Server Error"))<=0){
								printf("fprintf\n");exit(1);
							}
							addlog(un,500,sizeof(command),i); 
						}
						
					}
					break;
				}
				sigaction(SIGINT, &saOrigInt,0);
   				sigaction(SIGQUIT, &saOrigQuit,0);
				close(fd);
				return status;
}

void addlog( char* ligne, int code, int size,int fd)
{	
	
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);
    char ip[30];
    struct timeval  tv;
    char date[30];
    int pid_log;
	int  tid_log;
	int i;
	int cpt=0;
	 char tampon[1000];
	
	 if (getpeername(fd, (struct sockaddr *)&addr, &addr_len) ||!inet_ntop(AF_INET, &addr.sin_addr, ip, sizeof(ip)))
    strcpy(ip,"null");

   if (gettimeofday(&tv, NULL) || !ctime_r(&tv.tv_sec, date)){
     	strcpy(date, "null");
    	date[strlen(date)-1] = 0;
	}
	pid_log = (int)getpid();
    tid_log = (int)pthread_self();
	int number;
 	number =  snprintf(tampon, sizeof(tampon), "adresse IP: %s date: %s PID: %d identifiant de la Thread:%d :première ligne:%s code de retour: %d  taille du fichier: %d\n",ip,date,pid_log,tid_log,ligne, code, size);
		printf("bbbb%s\n",tampon);

   pthread_mutex_lock(&mutex);
		FILE* flog;
		if((flog = fopen("tmp/http3605744.log", "a"))==-1){
			return 0;
	   }
		lseek(fileno(flog),NULL,SEEK_END);
	    if((write(fileno(flog),tampon,number))== -1){
		    perror("write error");
		    return errno;
        }
	pthread_mutex_unlock(&mutex);
  
}


char* fichiertype(char* chemin)
{
  int len = strlen(chemin);
  if (!strcasecmp(chemin+len-5, ".html") ||!strcasecmp(chemin+len-4, ".htm")) return "text/html";
  if (!strcasecmp(chemin+len-4, ".css")) return "text/css";
  if (!strcasecmp(chemin+len-4, ".png")) return "image/png";
  if (!strcasecmp(chemin+len-4, ".gif")) return "image/gif";
  if (!strcasecmp(chemin+len-5, ".jpeg") ||!strcasecmp(chemin+len-4, ".jpg")) return "image/jpeg";
  return "text/ascii";
}

void existpas(char* un,int fd)
{
	 if((dprintf(fd, "HTTP/1.1 404 Not found\r\n"))<=0){
			printf("dprintf\n");exit(1);
		}
  	addlog(un,404,NULL,fd);
  	printf("%s\n","404 not found");
  	close(fd);
  	pthread_exit((void*)0);
}

void forbidden(char* un,int fd)
{
	if((dprintf(fd, "%s\r\n","HTTP/1.1 403 Forbidden"))<=0){
			printf("dprintf\n");exit(1);
		}
	
 	addlog(un,403,NULL,fd);
  	printf("%s\n","403 Forbidden");
  	close(fd);
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
	
 	 while (1) {                     //boucle pour recuperer chaque commande
				 
				struct stat s;
				long fich_size;
				int j,k,fd;
				
			  	if((fgets(un, 100, fich))<=0){
					printf("%s\n","fgets un");
		  			fclose(fich);
		  			pthread_exit((void*)0);
				}
			   if (un[0]=='\n' ||un[0]=='\r'){
					continue;
				 }
				if (!strncasecmp(un, "Connection:", 11) || strstr(un, "close")){
						break;
					}
			   if ((strncmp(un, "GET", 3))!=0){
			  		printf("%s\n","GET error");
		  			fclose(fich);
		  			pthread_exit((void*)0);
			   } 
		                    
			  for (j=3; un[j]==' '; j++);
			  for (;un[j] && un[j]!='/'; j++);
			  if (un[j] =='/') j += 5 ;
			  for (k=0; un[j] && un[j]!=' '; k++, j++){  //le chemin de fichier ne contient pas d'espace
				chemin[k] = un[j];
			  }
			  chemin[k] = 0;
			  for (; un[j]==' '; j++);
			  if ((strncmp(un+j, "HTTP/1.1", 8))!=0){
					printf("%s\n","HTTP error");
			  		fclose(fich);
			  		pthread_exit((void*)0);
			  }
				
	
			  if((fgets(deux, 100, fich))<=0){
					printf("%s\n","fgets deux");
		  			fclose(fich);
		  			pthread_exit((void*)0);
				}
			  if ((strcmp(deux, "Host: 127.0.0.1"))!=0){
			  		printf("%s\n","ligne deux error");
		  			fclose(fich);
		  			pthread_exit((void*)0);
			  } 
				
			   if((fd = open(chemin, O_RDONLY))==-1){
					existpas(un,i); 
					return 0;
			  }
			  if ((access(chemin,F_OK)) == -1){
					 close(fd); 
					existpas(un,i); 
					return 0; 
				}
			  if (fstat(fd, &s)==-1 || !S_ISREG(s.st_mode) ){
					 close(fd); 
					existpas(un,i); 
					return 0; 
				}  
		
		 	  if ( !(s.st_mode & S_IROTH)){
					 close(fd); 
					forbidden(un,i); 
					return 0; 
				} 
			 
			  fich_size = (long)s.st_size;
		    addlog(un,200,fich_size,i);
		
			if(dprintf(i, "%s\r\n","HTTP/1.1 200 OK")<=0){
					printf("errrr\n");exit(1);
				}
			
			if((dprintf(i, "Content-type: %s\r\n", fichiertype(chemin)))<=0){
						printf("dprintf\n");exit(1);
				}
		    if((dprintf(i, "\r\n"))<=0){
					printf("dprintf\n");exit(1);
				}
	  while (1) {
		int csize;
		int m;
		int cpt;
		char context[5000];
		if((csize = read(fd,context,sizeof(context))) <=0){
				close(fd);
				  printf("%s\n","read context");
			  	  fclose(fich);
			  	  pthread_exit((void*)0);
			}
		if (csize==0) break;
		if (csize<0) {
		  
			  close(fd);
			printf("%s\n","read context");
		  	fclose(fich);
		  	pthread_exit((void*)0);
		}
		 fflush(fich);
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
		int ret;
			if ((access(chemin,X_OK)) == 0){	 
				if(ret = (execute(un,i,context))!=0){
					if((fprintf(fich, "%s\r\n","HTTP/1.1 500 Internal Server Error"))<=0){
									printf("fprintf\n");exit(1);
					}
					addlog(un,500,csize,i); 
				
		       }
			 }
		
       }

        close(fd);
	}
	 fclose(fich);
	return 0;
  }
	

int main(int argc, char *argv[])
{
	struct sockaddr_in sin; 
	struct sockaddr_in exp; 
	int on = 1;
	char host[64];

    int sc;
	int scom;
	int fd;
	int fromlen = sizeof(exp);

	if((sc = socket(AF_INET,SOCK_STREAM,0))<0){
		perror("socket");exit(1);	
	}
	if((setsockopt(sc,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on)))==-1){
		perror("setsockopt");exit(1);	
	}
	
	memset(&sin,0,sizeof(sin));
	sin.sin_port = htons(atoi(argv[1]));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = htonl(INADDR_ANY); 
	if(bind(sc,(struct sockaddr *)&sin,sizeof(sin))<0){
		perror("bind");exit(1);	
	}
	listen(sc,atoi(argv[2]));
	
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
