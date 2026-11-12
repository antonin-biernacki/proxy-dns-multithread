#include <getopt.h>

#include "../libs/Reseau/libreseau.h"

#define PORT_DEFAUT "53"


int main(int argc, char* argv[]) {
	args_init_t args_init;
	int option;
	strcpy(args_init.port, PORT_DEFAUT);
	strcpy(args_init.serveur, NAMESERVER_GOOGLE);
	strcpy(args_init.strategie, "\0");
	strcpy(args_init.fichier_logs, "\0");


	static struct option options_longues[] = {
										{"port", required_argument, 0, 'p'},
										{"serveur", required_argument, 0, 's'},
										{"logstrategie", required_argument, 0, 'l'},
										{"initlogstrategie", required_argument, 0, 'i'},
										{"help", no_argument, 0, 'h'},
										{0, 0, 0, 0}
	};


	while ((option = getopt_long(argc, argv, "l:i:p:s:h", options_longues, NULL)) != -1) {
		if (option == 'l') {
			snprintf(args_init.strategie, TAILLE, "%s", optarg);
		} else if (option == 'i') {
			snprintf(args_init.fichier_logs, TAILLE, "%s", optarg);
		} else if (option == 'p') {
			snprintf(args_init.port, TAILLE, "%s", optarg);
		} else if (option == 's') {
			snprintf(args_init.serveur, TAILLE, "%s", optarg);
		} else if (option == 'h') {
			printf(
				"Ce programme lance un proxyDNS\n"
				"Commande : \n"
				"	%s [OPTIONS]\n"
				"OPTIONS :\n"
				"	--help, -h : Affiche ce message\n"
				"	--port, -p <port> : Port d'écoute du proxyDNS (optionnel, par défaut 53)\n"
				"	--serveur, -s <adresse_serveur> : Adresse du serveur DNS (optionnel, par défaut google 8.8.8.8)\n"
				"	--logstrategie, -l [STRATEGIE] : Charge la stratégie (optionnel)\n"
				"	--initlogstrategie, -i <fichier> : Sauvergarde les logs dans fichiers (optionnel)\n"
				"STRATEGIES :\n"
				"	libafficheLog.so : affiche le nom de domaine des requêtes\n"
				"	libsaveLog.so :	sauvegarde les requêtes dans le fichier donné en parametre via -i\n"
				"Notes :\n"
				"	Un port inférieur à 1024 (dont 53 par défaut) nécessite sudo : sudo %s\n",
				argv[0], argv[0]);
			exit(EXIT_SUCCESS);
		}
	}


	int s = initialisationSocketUDP(args_init.port);

	printf("   Port utilisé : %s\n", args_init.port);
	printf("   Serveur utilisé : %s\n", args_init.serveur);
	if (args_init.strategie[0] == '\0') {
		printf("   Pas de stratégie chargée \n");
	} else {
		printf("   Chargement de la stratégie : %s\n", args_init.strategie);
		if (!strcmp(args_init.strategie, "libsaveLog.so")) {
			printf("   Fichier de sauvegarde des logs : %s\n", args_init.fichier_logs);
		}
	}

	boucleServeurUDP(args_init, s, messageUDP);
	close(s);
	return 0;
}
