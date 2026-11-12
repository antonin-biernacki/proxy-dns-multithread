#include "log_dns.h"

FILE * fichier = NULL;

int initLog(void *param) {
	char *nom_fichier = (char *)param;
	if (nom_fichier == NULL || nom_fichier[0] == '\0') {
		printf("   Aucun fichier de logs donné (option -i)\n");
		return -1;
	}
	fichier = fopen(nom_fichier, "a");
	if (!(fichier)) {
		return -1;
	}
	return 0;
}


// Une ligne par message, octet par octet en hexadecimal
int addLog(logMsg_t *requete) {
	if (fichier == NULL) {
		return -1;
	}
	for (uint32_t i=0 ; i<requete->longueur ; i++) {
		fprintf(fichier, "%02x", requete->octets[i]);
	}
	fprintf(fichier, "\n");
	fflush(fichier);
	printf("   Requête sauvegardée\n");
	return 1;
}
