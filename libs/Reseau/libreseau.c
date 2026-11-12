#include "libreseau.h"


int initialisationSocketUDP(char *service) {
	int sockfd;
	struct sockaddr_in addr;
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);	

	int opt = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi(service));
	addr.sin_addr.s_addr = INADDR_ANY;
	
	if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		perror("bind (port déjà utilisé ou < 1024 sans sudo ?)");
		exit(EXIT_FAILURE);
	}
	printf(">> Serveur demarre \n");
	return sockfd;
}


void* messageUDP(void* args_thread) {
	argsRequete_t* args = (argsRequete_t*)args_thread;

	// Envoi la requete au DNS
	int s = socket(AF_INET, SOCK_DGRAM, 0);
	// Sans reponse du serveur au bout de TIMEOUT_DNS secondes, le thread abandonne
	struct timeval timeout = {TIMEOUT_DNS, 0};
	setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
	// On envoie uniquement les octets recus du client, pas tout le buffer
	sendto(s, args->question, args->taille, 0, (struct sockaddr *)&args->dnsAddr, args->dnsLen);
	// Revoit la reponse du DNS
	int rlen = recvfrom(s, args->reponse, TAILLE, 0, (struct sockaddr *)&args->dnsAddr, &args->dnsLen);
	close(s);

	if (rlen < 0) {
		printf("   Pas de réponse du serveur DNS \n");
		free(args);
		return NULL;
	}

	// Envoi la reponse au client 
	sendto(args->s, args->reponse, rlen, 0, (struct sockaddr *)&args->clientAddr, args->clientLen);
	printf("   Requête traitée \n");
	printf("\n\n");
	free(args);
	return NULL;
}


int boucleServeurUDP(args_init_t args_init, int s, void* (*messageUDP)(void* arg_thread)) {
	argsLog_t args_log;
	// Initialisation des stratégies
	if (args_init.strategie[0] == '\0') {
		// printf("   Pas de stratégie chargée \n");
	} else {
		strcpy(args_log.strategie, args_init.strategie);
		strcpy(args_log.fichier_logs, args_init.fichier_logs);
		// printf("   Chargement de la stratégie : %s\n", args_log.strategie);
		launchThread(loadStrategy, &args_log, sizeof(args_log));
	}
	// Initialisation de la mémoire partagée
	if (allocateMemory(1024) < 0) {
		printf("   Probleme lors de l'initialisation de la mémoire\n");
	}
	
	
	struct sockaddr_in clientAddr, dnsAddr;
	socklen_t clientLen = sizeof(clientAddr);
	socklen_t dnsLen = sizeof(dnsAddr);

	char question[TAILLE];
	int nb_octets;

	memset(&dnsAddr, 0, sizeof(dnsAddr));
	dnsAddr.sin_family = AF_INET;
	// le serveur amont ecoute toujours sur 53, -p ne concerne que l'ecoute locale
	dnsAddr.sin_port = htons(DNS_PORT_DISTANT);
	inet_aton(args_init.serveur, &dnsAddr.sin_addr);

	argsRequete_t args;
	printf("\n\n");
	while(1) {
		// printf("\n>> En attente d'une requete \n");
		clientLen = sizeof(clientAddr);
		nb_octets = recvfrom(s, question, TAILLE, 0, (struct sockaddr *)&clientAddr, &clientLen);
		if (nb_octets <= 0) {
			continue;
		}
		printf(">> Requête client reçue \n");


		args.s = s;
		// memcpy et pas strcpy : un paquet DNS contient des octets nuls
		memcpy(args.question, question, nb_octets);
		args.taille = nb_octets;
		args.clientAddr = clientAddr;
		args.dnsAddr = dnsAddr;
		args.clientLen = clientLen;
		args.dnsLen = dnsLen;

	
		launchThread(messageUDP, &args, sizeof(args));


		if (writeMemory(question, nb_octets) < 0) {
			printf(">> Memoire saturée impossible d'écrire\n");
		}
	}
}
