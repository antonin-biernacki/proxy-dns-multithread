#include "genericLog.h"

void *handle = NULL;


// Corps du thread de log : charge la strategie puis traite les messages de la memoire partagee
void* loadStrategy(void* args) {
    argsLog_t* args_log = (argsLog_t*)args;
    handle = dlopen(args_log->strategie, RTLD_LAZY);
    if (handle == NULL) {
        printf("   Impossible de charger la stratégie : %s\n", dlerror());
        free(args);
        return NULL;
    }

    // Initialisation une seule fois (ouverture du fichier de logs par exemple)
    if (initStrategy(args_log->fichier_logs) < 0) {
        printf("   Echec de l'initialisation de la stratégie\n");
    }
    free(args);

    logMsg_t requete;
    while (1) {
        requete.octets = readMemory(&requete.longueur);    // bloquant
        if (requete.longueur > 0) {
            logStrategy(&requete);
        }
        free(requete.octets);
    }
    return NULL;
}


int initStrategy(void * fichier) {
    int (*initialisation)(void *);
    *(void **)(&initialisation) = dlsym(handle, "initLog");
    if (initialisation == NULL) {
        return -1;
    }
    return initialisation(fichier);
}


int logStrategy(logMsg_t *requete) {
    int (*ajouter)(logMsg_t *);
    *(void **)(&ajouter) = dlsym(handle, "addLog");
    if (ajouter == NULL) {
        return -1;
    }
    return ajouter(requete);
}
