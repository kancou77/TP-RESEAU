#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#define PORT_TCP 1234
#define PORT_UDP 2223
#define ADDR "0.0.0.0"
#define ADDR_UDP "172.16.130.178"
#define BUFSIZE 512
#define BUFSIZE1 100

void stop(char *msg) {
 perror(msg);
 exit(EXIT_FAILURE);
}

int main ( int argc, char ** argv) {

 char buffer_reception[BUFSIZE1];
 char buffer_envoie[BUFSIZE];

 // Création de la socket TCP du serveur
 int socketserver = socket (AF_INET, SOCK_STREAM,0);
 if (socketserver == -1)
   stop("socket");

 //On rajoute l'option qui nous permet de réutiliser la socket
 int option = 1;
 setsockopt(socketserver, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
 struct sockaddr_in addrserver;
 memset(&addrserver, 0, sizeof(addrserver)); //On initialise la struct

 addrserver.sin_family = AF_INET;
 addrserver.sin_port = htons(PORT_TCP); 
 inet_aton(ADDR, &addrserver.sin_addr);

 if(bind(socketserver,(struct sockaddr *) &addrserver , sizeof(addrserver))==-1){
	 close(socketserver);
	 stop("bind");
 }

 if(listen(socketserver,5)==-1){
	 close(socketserver);
	 stop("listen");
}

printf("Attente de connexion\n"); // Serveur prêt
int sockclient = accept(socketserver, NULL, NULL); // Socket nous permattant de communiquer avec le client

printf("Client connecté\n "); // Client connecté

if(sockclient == -1) {
	close(socketserver);
	stop("accept");
}

// On initialise le buffer de reception et d'envoie
memset(buffer_reception, 0, sizeof(buffer_reception));
memset(buffer_envoie,0,sizeof(buffer_envoie));


int recu = recv(sockclient, buffer_reception, 100, 0);
//Gestion erreur de recv
if(recu == -1) {
	close(sockclient);
	close(socketserver);
	stop("recv");
}

if(recu==0) {
	printf("Client déconnecté\n");
	exit(EXIT_FAILURE);
}

printf("Données reçues %s\n", buffer_reception);
snprintf(buffer_envoie, BUFSIZE, "%s TCPs1",buffer_reception);
//On ferme les sockets dont on a plus besoin
close(sockclient);
close(socketserver);

int sockudp = socket(AF_INET,SOCK_DGRAM,0);
if(sockudp==-1)
	stop("socket");

struct sockaddr_in addrudp;
addrudp.sin_family = AF_INET;
addrudp.sin_port = htons(PORT_UDP);
inet_aton(ADDR_UDP,&addrudp.sin_addr);


int send = sendto(sockudp,buffer_envoie,sizeof(buffer_envoie),0,(struct sockaddr *)&addrudp, sizeof(addrudp));
if(send <0) {
	close(sockudp);
	stop("sendto");
}

printf("Données envoyées vers s2 \n");

//On ferme la socket udp
close(sockudp);

return 0;
}

