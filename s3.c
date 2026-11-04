#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#define PORT_RECU 3456
#define ADDR "0.0.0.0"
#define BUFSIZE 512
void stop(char *msg) {
 perror(msg);
 exit(EXIT_FAILURE);
}

int main ( int argc, char ** argv) {

 char buffer_reception[128];
 char buffer_envoie[BUFSIZE];
 //Création de la socket de reception du message venant de s2
 int socketserver = socket (AF_INET, SOCK_DGRAM,0);
 if (socketserver == -1)
   stop("socket");

 struct sockaddr_in addrserver;
 memset(&addrserver, 0, sizeof(addrserver));

 addrserver.sin_family = AF_INET;
 addrserver.sin_port = htons(PORT_RECU);
 inet_aton(ADDR, &addrserver.sin_addr);

 if(bind(socketserver,(struct sockaddr *) &addrserver , sizeof(addrserver))==-1){
	 close(socketserver);
	 stop("bind1");
 }

memset(buffer_reception, 0, sizeof(buffer_reception));
memset(buffer_envoie,0,sizeof(buffer_envoie));

int recu = recvfrom(socketserver, buffer_reception, 100, 0,NULL,NULL);
if(recu == -1) {
	close(socketserver);
	stop("recv");
}


printf("Données reçues\n");
//On concaténe le message reçu avec ce u'on veut ajouter

snprintf(buffer_envoie, BUFSIZE, "%s UDPs3",buffer_reception);

//On ferme la socket UDP
close(socketserver);

//On crée la socket TCP permettant l'envoie des données vers c1
int socktcp = socket(AF_INET,SOCK_STREAM,0);
if(socktcp==-1)
	stop("socket");

struct sockaddr_in addrtcp;
addrtcp.sin_family = AF_INET;
addrtcp.sin_port = htons(PORT_RECU);
inet_aton(ADDR, &addrtcp.sin_addr);

//On rajoute l'option permettant de réutiliser la socket
int option = 1;
 setsockopt(socktcp, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
if(bind(socktcp, (struct sockaddr *)&addrtcp,sizeof(addrtcp))==-1) {
	close(socktcp);
	stop("bind");
}

if(listen(socktcp,5)==-1){
	close(socktcp);
	stop("listen");
}

printf("Attente de connexion\n"); //Serveur prêt 

int sockclient = accept(socktcp, NULL, NULL);

printf("Client connecté\n");

int env = send(sockclient,buffer_envoie,sizeof(buffer_envoie),0);
if(env <0) {
	close(socktcp);
	stop("send");
}

printf("Données envoyées au client c1 \n");

close(socktcp);

return 0;
}

