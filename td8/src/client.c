#define _XOPEN_SOURCE 700


/** Client emetteur de fichiers pas TCP **/

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>

int main(int argc, char *argv[])
{
    struct sockaddr_in dest; /* Nom du serveur */
    struct hostent *hp; 
    int sock;
	char* nomfich;

    if ((sock = socket(AF_INET,SOCK_STREAM,0)) == -1) {
        perror("socket");
        exit(1);
    }

    /* Remplir la structure dest */

    if ((hp = gethostbyname(argv[1])) == NULL) {
        perror("gethostbyname");
        exit(1);
    }
    memset((void *)&dest,0, sizeof(dest));
    memcpy((void*)hp->h_addr_list[0],(void*)&dest.sin_addr,hp->h_length);
    dest.sin_family = AF_INET;
    dest.sin_port = htons(7100);

    /* Etablir la connexion */
    if (connect(sock, (struct sockaddr *) &dest, sizeof(dest)) == -1) {
        perror("connect");
        exit(1);
    }

    nomfich = "test";
	
	if(write(sock,nomfich,sizeof(nomfich))==-1){
		perror("write"); exit(1);
	}

    /* Fermer la connexion */
    shutdown(sock,2);
    close(sock);
    return(0);
}
