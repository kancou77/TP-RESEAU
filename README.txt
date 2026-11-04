# TP-RESEAU

Membre 1 : OUBIHI Wael
Membre 2 : THIAM Adama

Fonctionnement :

1- Le client c1 envoie le message "Bonjour" via TCP au serveur s1.
2- Le serveur s1  envoie via UDP au serveur s2 le message "Bonjour TCPs1"
3- Le serveur s2 envoie via UDP au serveur s3 le message "Bonjour TCPs1 UDPs2"
4- Le serveur s3 envoie via UDP au client c1 le message "Bonjour TCPs1 UDPs2 UDPs3"

5- Le client c1 affiche le message final "Bonjour TCPs1 UDPs2 UDPs3 TCPs1"

NB: Il faut lancer s1, s2 et s3 puis ensuite c1 !!!
