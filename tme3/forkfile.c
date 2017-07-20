#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

void sig_hand(int sig){

}

int main (void) {

	sigset_t sig_proc;
	struct sigaction action;
	sigemptyset(&sig_proc);
	action.sa_mask = sig_proc;
	action.sa_flags = 0;
	action.sa_handler = sig_hand;
	sigaction(SIGUSR1,&action,0);

    int fd1, fd2, fd3;
    if ((fd1 = open ("./fich1", O_RDWR| O_CREAT | O_TRUNC, 0600)) == -1) {
      perror("./fich1");
      return errno;
    }
    if (write (fd1,"abcde", strlen ("abcde")) == -1) {
      perror("./fich1");
      return errno;
    }
    if (fork () == 0) {
        if ((fd2 = open ("./fich1", O_RDWR)) == -1) {
	  perror("./fich1");
	  return errno;
	}
        if (write (fd1,"123", strlen ("123")) == -1) {
	  perror("./fich1");
	  return errno;
	}

	kill(getppid(),SIGUSR1);

        if (write (fd2,"45", strlen ("45")) == -1) {
	  perror("./fich1");
	  return errno;
	}
        close(fd2); 
    } else {
        fd3 = dup(fd1);

	sigfillset(&sig_proc);
	sigdelset(&sig_proc,SIGUSR1);
	sigsuspend(&sig_proc);

        if (lseek (fd3,0,SEEK_SET) == -1) {
	  perror("./fich1");
	  return errno;
	}
        if (write (fd3,"fg", strlen ("fg")) == -1) {
	  perror("./fich1");
	  return errno;
	}
	if (write (fd1,"hi", strlen ("hi")) == -1)  {
	  perror("./fich1");
	  return errno;
	}
        wait (NULL);
        close (fd1);
        close(fd3);
        return EXIT_SUCCESS;
    }
}
