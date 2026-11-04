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
void stop(char *msg) {
 perror(msg);
 exit(EXIT_FAILURE);
}

int main ( int argc, char ** argv) {

 char buffer_reception[100];
 char buffer_envoie[BUFSIZE];
 int socketserver = socket (AF_INET, SOCK_STREAM,0);
 if (socketserver == -1)
   stop("socket");

 struct sockaddr_in addrserver;
 memset(&addrserver, 0, sizeof(addrserver));

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

printf("Attente de connexion\n");
int sockclient = accept(socketserver, NULL, NULL);

printf("Client connecté\n ");
if(sockclient == -1) {
	close(socketserver);
	stop("accept");
}

memset(buffer_reception, 0, sizeof(buffer_reception));
memset(buffer_envoie,0,sizeof(buffer_envoie));

int recu = recv(sockclient, buffer_reception, 100, 0);
if(recu == -1) {
	close(sockclient);
	close(socketserver);
	stop("recv");
}

if(recu==0) {
	printf("Client déconnecté\n");
	exit(EXIT_FAILURE);
}

printf("J'ai reçu : %s\n", buffer_reception);
snprintf(buffer_envoie, BUFSIZE, "%s TCPs1",buffer_reception);

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

printf("Envoie vers s2 effectué\n");

close(sockudp);

return 0;
}

