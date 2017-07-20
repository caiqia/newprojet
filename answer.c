#define _XOPEN_SOURCE 700
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <time.h>
#include <errno.h>
#include <sys/types.h>
#include <aio.h>
#include <signal.h>
#include <string.h>

int nSignalRecu;
int som_attente;

void ignore_signal(int signo, siginfo_t *si, void *context){}

void attente_signal(int signo, siginfo_t *si, void *context){
    nSignalRecu++;
	som_attente+=si->si_value.sival_int;
	printf("recu : %d\n",si->si_value.sival_int);
}

void initiateur(int h,char *fichier){
	printf("Je suis %d, initiateur, h=%d\n",getpid(),h);
	// Réception d'un signal par descendant et somme des temps d'attente
	int nb_desc=(1<<(h+1))-2;
	struct sigaction action;
	sigset_t mask,mask_suspend;
    sigemptyset(&mask);
    sigaddset(&mask, SIGRTMIN);
    action.sa_sigaction=attente_signal;
    action.sa_flags=SA_SIGINFO;
    action.sa_mask=mask;
    sigaction(SIGRTMIN, &action, 0);
    sigprocmask(SIG_SETMASK, &mask, NULL);
	sigfillset(&mask_suspend);
    sigdelset(&mask_suspend,SIGRTMIN);
	nSignalRecu=0;
	som_attente=0;
    while(1){
		sigsuspend(&mask_suspend);
        if(nSignalRecu==nb_desc){
            break;
        }
    }	
	printf("Inititateur a recu %d signaux, moyenne temps attente = %f\n",nb_desc,((float)som_attente)/nb_desc);
	// Déblocage des descendants par signal (pid récupérés dans le fichier partagé)
	union sigval val;
	int fic=open(fichier,O_RDONLY);
	pid_t lu;
	while(read(fic,&lu,sizeof(pid_t))==sizeof(pid_t)){
		sigqueue(lu, SIGRTMIN, val);
	}
	close(fic);
}

void descendant(int h,char *fichier,pid_t pid_init){
	// Présentation et attente
	printf("Je suis %d, fils de %d\n",getpid(),getppid());
	srand(getpid());
	int attente=rand()%10+1;
	sleep(attente);
	// Inscription du pid dans le fichier partagé (attention : verrou)
	int fic=open(fichier,O_RDWR|O_APPEND,S_IRUSR|S_IWUSR);
	struct flock lock;
	lock.l_start=0;
	lock.l_len=0;
	lock.l_type=F_WRLCK;
	fcntl(fic,F_SETLKW,&lock);
	pid_t mypid=getpid();
	write(fic,&mypid,sizeof(pid_t));
	lock.l_type=F_UNLCK;
	fcntl(fic,F_SETLK,&lock);
	close(fic);
	// Envoi du signal RT au proc initiateur avec valeur de l'attente
	union sigval val;
	val.sival_int = attente;
	if(sigqueue(pid_init, SIGRTMIN, val)==-1){
        perror("sigqueue");
        exit(EXIT_FAILURE);
    }
	// Attente du déblocage de la barrière par le proc initiateur
	struct sigaction action;
	sigset_t mask,mask_suspend;
    sigemptyset(&mask);
    sigaddset(&mask, SIGRTMIN);
    action.sa_sigaction=ignore_signal;
    action.sa_flags=SA_SIGINFO;
    action.sa_mask=mask;
    sigaction(SIGRTMIN, &action, 0);
    sigprocmask(SIG_SETMASK, &mask, NULL);
	sigfillset(&mask_suspend);
    sigdelset(&mask_suspend,SIGRTMIN);
	sigsuspend(&mask_suspend);
	printf("%d se termine !\n",getpid());
}

int main(int argc, char *argv[]){
	if(argc!=3){
		printf("Pb nb arguments\n");
		exit(EXIT_FAILURE);
	}

	int h=atoi(argv[1]);
	char *fichier=strdup(argv[2]);
	int fic=open(fichier,O_RDWR|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR);
	close(fic);
	pid_t pid_init=getpid();
	// Création arborescence
	while(h>0){
		if(fork()>0&&fork()>0){
			if(getpid()==pid_init){
				initiateur(h,fichier);
				exit(EXIT_SUCCESS);
			}else{
				descendant(h,fichier,pid_init);
				exit(EXIT_SUCCESS);
			}
		}
		h--;
	}
	descendant(h,fichier,pid_init);
	exit(EXIT_SUCCESS);
}
