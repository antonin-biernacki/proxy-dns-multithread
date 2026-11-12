#include "log_dns.h"

#define POSITION_QR 0x80 //permet de selectionner le premier bit du 3eme octet donc QR 10 00 00 00
#define DEBUT_QNAME 12


int initLog(void* param) {
	(void)param;
	return 0;
}


int addLog(logMsg_t* requete) {
	if (requete->longueur <= DEBUT_QNAME) {
		return -1;
	}
	// QR : question=0 reponse=1
	// QR 16 bits => 3eme octets => 1er bit 3eme octets 
	uint8_t octet_3 = requete->octets[2];
	if ((octet_3 & POSITION_QR) == 0) {
		// c'est une question
		// 12 octets d'entet donc le domaine sera pas dans les 12 premiers octets
		// QNAME contient nos noms de domaines
		/* Explication pour extraire les noms de domaines de QNAME (ex avec google.com)
			06 (google) lire les 6 octets suivants puis ajouter un .
			03 (com) lire les 3 octets suivants
			s'arreter a 0x00
		*/
		uint16_t i = DEBUT_QNAME;
		printf("   Nom de domaine de la requête : ");
		while (i < requete->longueur && requete->octets[i] != 0x00) {
			uint8_t l = requete->octets[i];
			i++;
			for (uint16_t j = i; j < i + l && j < requete->longueur; j++) {
				printf("%c", requete->octets[j]);
			}
			i = i + l;
			if (i < requete->longueur && requete->octets[i] != 0x00) {
				printf(".");
			}
		}
		printf("\n");
	}
	return 0;
}
