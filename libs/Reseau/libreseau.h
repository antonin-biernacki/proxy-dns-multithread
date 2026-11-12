#ifndef LIBRESEAU_H
#define LIBRESEAU_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/time.h>

#include "dns_server.h"
#include "../Log/genericLog.h"
#include "../Threads/libthread.h"
#include "../Memoire/libmemoire.h"

#define DNS_PORT_DISTANT 53
#define TAILLE 512
#define TIMEOUT_DNS 3


typedef struct {
    int s;
    int taille;
    char question[TAILLE];
    char reponse[TAILLE];
    struct sockaddr_in clientAddr;
    struct sockaddr_in dnsAddr;
    socklen_t clientLen;
    socklen_t dnsLen;
} argsRequete_t;



typedef struct {
    char port[TAILLE];
    char serveur[TAILLE];
    char strategie[TAILLE];
    char fichier_logs[TAILLE];
} args_init_t;


int initialisationSocketUDP(char *service);
void* messageUDP(void* args_thread);
int boucleServeurUDP(args_init_t args_init, int s, void* (*messageUDP)(void* arg));


#endif
