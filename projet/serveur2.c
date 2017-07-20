/* Correction du TP de programmation système UNIX

   TP serveur HTTP multi-thread

   Question 2

   Réponse basée sur la question 1
   Les modifications sont indiquées par XXX

   -
   Antoine Miné
   06/05/2007
 */

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


/* affiche un message d'erreur, errno et quitte */
void fatal_error(const char* msg)
{
  fprintf(stderr, "%s (errno: %s)\n", msg, strerror(errno));
  exit(1);
}


/* XXX structure de données utilisée pour le journal */

#define LOGSIZE 1024

char* logdata[LOGSIZE];

pthread_mutex_t log_mutex;


/* XXX ajoute une entrée au journal */
void add_log(char* msg)
{
	
  int i;
  errno = pthread_mutex_lock(&log_mutex);
  if (errno) fatal_error("échec de pthread_mutex_lock");

  /* cherche une entrée vide */
  for (i=0; i<LOGSIZE && logdata[i]; i++);
  if (i==LOGSIZE) {
    /* le journal est plein => on le vide */
    for (i=0; i<LOGSIZE; i++) {
      free(logdata[i]);
      logdata[i] = NULL;
    }
    i = 0;
  }

  /* ajout de l'entrée */
  logdata[i] = strdup(msg);

  errno = pthread_mutex_unlock(&log_mutex);
  if (errno) fatal_error("échec de pthread_mutex_unlock");
}

/* XXX affiche le journal au format HTML */
void affiche_log(FILE* stream)
{
	printf("log\n");
  int i;
	fprintf(stream, "Connection: close\r\n");
  fprintf(stream, "HTTP/1.1 200 OK\r\n");
  fprintf(stream, "Connection: close\r\n");
  fprintf(stream, "Content-type: text/html\r\n");
  fprintf(stream, "Cache-control: no-cache\r\n");
  fprintf(stream, "\r\n");
  fprintf(stream, 
	  "<html><head><title>Server log</title>"
	  "<META HTTP-EQUIV=\"Refresh\" CONTENT=\"15\">"
	  "</head>\n<body><h1>Server log</h1><ul>\n");

  errno = pthread_mutex_lock(&log_mutex);
  if (errno) fatal_error("échec de pthread_mutex_lock");

  for (i=0; i<LOGSIZE && logdata[i]; i++) 
    fprintf(stream, "<li>%s\n", logdata[i]);
 
  errno = pthread_mutex_unlock(&log_mutex);
  if (errno) fatal_error("échec de pthread_mutex_unlock");

  fprintf(stream, "</ul></body></html>\n");
}



/* crée une socket d'écoute sur le port indiqué 
   retourne son descripteur 
*/
int cree_socket_ecoute(int port)
{
  int listen_fd;           /* socket */
  struct sockaddr_in addr; /* adresse IPv4 d'écoute */
  int one = 1;             /* utilisé avec setsockopt */
  
  /* crée une socket TCP */
  listen_fd = socket(PF_INET, SOCK_STREAM, 0);
  if (listen_fd==-1) fatal_error("échec de socket");
  
  /* évite le délai entre deux bind successifs sur le même port */
  if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one))==-1)
    fatal_error("échec de setsockopt(SO_REUSEADDR)");

  /* remplit l'adresse addr */
  memset(&addr, 0, sizeof(addr));  /* initialisation à 0 */
  addr.sin_family = AF_INET;       /* adresse Internet */
  addr.sin_port   = htons(port);   /* port: 2 octets en ordre de réseau */
  /* on accepte les connections sur toutes les adresses IP de la machine */
  addr.sin_addr.s_addr = htonl(INADDR_ANY); 

  /* ancre listen_fd à l'adresse addr */
  if (bind(listen_fd, (const struct sockaddr*) &addr, sizeof(addr))==-1)
    fatal_error("échec de bind");

  /* transforme liste_fd en une socket d'écoute */
  if (listen(listen_fd, 15)==-1) 
    fatal_error("échec de listen");
  
  fprintf(stderr, "Serveur actif sur le port %i\n", port);

  return listen_fd;
}

/* termine la connection: ferme la socket et termine la thread */
void fin_connection(FILE* stream, char* msg)
{
  fprintf(stderr, "fin de connection (%s, errno=%s)\n", msg, strerror(errno));
  fclose(stream);
  pthread_exit(NULL);
}

/* lit une ligne terminée par \r\n */
void my_fgets(char* buf, int size, FILE* stream)
{
  if (!fgets(buf, size-1, stream)) fin_connection(stream, "échec de fgets");
}


/* XXX ajoute une ligne de journal correspondant à la requête */
void log_requete(FILE* stream, char* url, char* msg)
{
  struct sockaddr_in addr;
  socklen_t addr_len = sizeof(addr);
  char ip[30];
  struct timeval  tv;
  char curtime[30];
  char buf[2096];

  /* adresse IP */
  if (getpeername(fileno(stream), (struct sockaddr *)&addr, &addr_len) ||
      !inet_ntop(AF_INET, &addr.sin_addr, ip, sizeof(ip)))
    strcpy(ip,"???");
  printf("ip %s\n",ip);
  /* date */
  if (gettimeofday(&tv, NULL) || !ctime_r(&tv.tv_sec, curtime))
    strcpy(curtime, "???\n");
  curtime[strlen(curtime)-1] = 0;
printf("curtime %s\n",curtime);
  /* génère la ligne complète */
  snprintf(buf, sizeof(buf), "%s: %s",curtime, ip);//: %s (%s)", curtime, ip, url, msg );
	printf("bbbb%s\n",buf);
  add_log(buf);
}


/* lit une ligne de la forme 
     GET /chemin HTTP/1.1
   et stocke 'chemin' dans le buffer url de taille size
*/
void lit_get(FILE* stream, char* url, int size)
{
  char buf[4096];
  int i,j;
  
  /* lit la requête */
  my_fgets(buf, sizeof(buf), stream);
  
  /* extrait l'URL et stocke-la dans url */
  if (strncmp(buf, "GET", 3)) {
    log_requete(stream, buf, "unrecognized request");
    fin_connection(stream, "la requête n'est pas GET");
  }
  for (i=3; buf[i]==' '; i++);
 // if (!strncmp(buf+i, "http://", 7)) i+=7;
  for (; buf[i] && buf[i]!='/'; i++);
  if (buf[i]=='/') i +=5;
  for (j=0; buf[i] && buf[i]!=' ' && j<size-1; j++, i++) url[j] = buf[i];
  url[j] = 0;
  for (; buf[i]==' '; i++);
  if (strncmp(buf+i, "HTTP/1.1", 8)) {
    log_requete(stream, buf, "unrecognized HTTP version");
    fin_connection(stream, "la version n'est pas HTTP/1.1");
  }
}

/* lit les en-têtes 
   renvoie 0 si l'en-tête 'connection: close' a été trouvée, 1 sinon
 */
int lit_en_tetes(FILE* stream)
{
  char buf[4096];
  int keepalive = 1;

  while (1) {
    my_fgets(buf, sizeof(buf), stream);
    
    /* fin des en-têtes */
    if (buf[0]=='\n' || buf[0]=='\r') break;
    
    /* détecte l'en-tête 'Connection: close' */
    if (!strncasecmp(buf, "Connection:", 11) ||
	strstr(buf, "close"))
      keepalive = 0;
  }
  
  return keepalive;
}

/* envoie une erreur 404 et ferme la connection */
void envoie_404(FILE* stream, char* url)
{
  log_requete(stream, url, "not found");
  fprintf(stream, "HTTP/1.1 404 Not found\r\n");
  fprintf(stream, "Connection: close\r\n");
  fprintf(stream, "Content-type: text/html\r\n");
  fprintf(stream, "\r\n");
  fprintf(stream, 
	  "<html><head><title>Not Found</title></head>"
	  "<body><p>Sorry, the object you requested was not found: "
	  "<tt>/%s</tt>.</body></html>\r\n", 
	  url);
  fin_connection(stream, "erreur 404");
}

/* devine le type MIME d'un fichier */
char* type_fichier(char* chemin)
{
  int len = strlen(chemin);
  if (!strcasecmp(chemin+len-5, ".html") ||
      !strcasecmp(chemin+len-4, ".htm")) return "text/html";
  if (!strcasecmp(chemin+len-4, ".css")) return "text/css";
  if (!strcasecmp(chemin+len-4, ".png")) return "image/png";
  if (!strcasecmp(chemin+len-4, ".gif")) return "image/gif";
  if (!strcasecmp(chemin+len-5, ".jpeg") ||
      !strcasecmp(chemin+len-4, ".jpg")) return "image/jpeg";
  return "text/ascii";
}

/* envoie la contenu du fichier */
void envoie_fichier(FILE* stream, char* chemin, int keepalive)
{
  char modiftime[30];
  char curtime[30];
  struct timeval  tv;
  char buf[4096];
  struct stat s;
  int fd;

  /* XXX cas spécial: on affiche le journal */
  if (!strcmp(chemin, "log")) {
    affiche_log(stream);
    fin_connection(stream, "ok: log");
  }

  /* pour des raisons de sécurité, on évite les chemins contenant .. */
  if (strstr(chemin,"..")) { envoie_404(stream, chemin); return; }

  /* ouverture et vérifications */
  fd = open(chemin, O_RDONLY);
  if (fd==-1) { envoie_404(stream, chemin); return; }
  if (fstat(fd, &s)==-1 || !S_ISREG(s.st_mode) || !(s.st_mode & S_IROTH))
    { close(fd); envoie_404(stream, chemin); return; }

  /* calcul des dates */
  if (gettimeofday(&tv, NULL) ||
      !ctime_r(&s.st_mtime, modiftime) ||
      !ctime_r(&tv.tv_sec, curtime))
     { close(fd); envoie_404(stream, chemin); return; }  
  modiftime[strlen(modiftime)-1] = 0; /* supprime le \n final */
  curtime[strlen(curtime)-1] = 0;     /* supprime le \n final */

  /* envoie l'en-tête */
  fprintf(stream, "HTTP/1.1 200 OK\r\n");fflush(stream);
  fprintf(stream, "Connection: %s\r\n", keepalive ? "keep-alive" : "close");fflush(stream);
  fprintf(stream, "Content-length: %li\r\n", (long)s.st_size);fflush(stream);
  fprintf(stream, "Content-type: %s\r\n", type_fichier(chemin));fflush(stream);
  fprintf(stream, "Date: %s\r\n", curtime);fflush(stream);
  fprintf(stream, "Last-modified: %s\r\n", modiftime);fflush(stream);
  fprintf(stream, "\r\n");fflush(stream);

  /* XXX ajoute une ligne au journal */
  log_requete(stream, chemin, "ok");

  /* envoie le corps */
  while (1) {
    int r = read(fd, buf, sizeof(buf)), w;
    if (r==0) break;
    if (r<0) {
      if (errno==EINTR) continue;
      close(fd);
      fin_connection(stream, "échec de read");
    }
    for (w=0; w<r; ) {
      int a = fwrite(buf+w, 1, r-w, stream);
      if (a<=0) {
	if (errno==EINTR) continue;
	close(fd);
	fin_connection(stream, "échec de write");
      }
      w += a;
    }
  }

  close(fd);
}

/* traitement d'une connection 
   lancé dans une thread séparée par boucle
 */
void* traite_connection(void* arg)
{
  int socket = (int)arg;
  FILE* stream = fdopen(socket, "r+"); /* transforme socket en FILE* */
  char url[4096];
  int keepalive = 1;

  errno = 0;

  /* bufferisation ligne par ligne */
  setlinebuf(stream);

  /* boucle de traitement des requêtes */
  while (keepalive) {

    /* lit la requête */
    lit_get(stream, url, sizeof(url));
    keepalive = lit_en_tetes(stream);

    /* envoie la réponse */
    printf("URL: %s\n",url);
    envoie_fichier(stream, url, keepalive);
  }

  /* fin normale */
  fin_connection(stream, "ok");
  return NULL;
}


/* boucle de réception des connections */
void boucle(int listen_fd)
{
  while (1) {
    int client;    /* socket connectée */
    pthread_t id;  /* thread qui va gérer la connection */

    /* attend une connection */
    client = accept(listen_fd, NULL, 0);
    if (client==-1) {
      if (errno==EINTR || errno==ECONNABORTED) continue; /* non fatal */
      fatal_error("échec de accept");
    }

    /* crée une nouvelle thread pour gérer la connection */
    errno = pthread_create(&id, NULL, traite_connection, (void*)client);
    if (errno) fatal_error("échec de pthread_create");

    /* évite d'avoir à appeler pthread_join */
    errno = pthread_detach(id);
    if (errno) fatal_error("échec de pthread_detach");
  }
}

int main()
{
  int i;

  /* XXX initialise le journal */
  errno = pthread_mutex_init(&log_mutex, NULL);
  if (errno) fatal_error("échec de pthread_mutex_init");
  for (i=0; i<LOGSIZE; i++) logdata[i] = NULL;
  add_log("server started");

  /* supprime SIGPIPE */
  signal(SIGPIPE, SIG_IGN);

  /* lance le serveur */
  boucle(cree_socket_ecoute(8080));

  return 0;
}
