#include "libmemoire.h"

/*
 * Memoire partagee entre les threads de requete et le thread de log.
 * C'est un tampon circulaire : les positions de lecture et d'ecriture reviennent
 * au debut du tampon quand elles atteignent la fin. Chaque message est stocke
 * precede de sa taille (2 octets) pour que le lecteur recupere les messages un
 * par un, meme si plusieurs se sont accumules.
 */

uint8_t* memoire = NULL;
size_t taille_memoire = 0;                                  // taille de la memoire
size_t positionLecture = 0;                                 // premier octet pas encore lu
size_t positionEcriture = 0;                                // premier octet libre
size_t octetsOccupes = 0;                                   // octets ecrits et pas encore lus
pthread_mutex_t mutex_memoire = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_memoire = PTHREAD_COND_INITIALIZER;     // signale qu'un message est disponible


// Copie size octets dans le tampon a partir de positionEcriture, en revenant au debut si besoin
static void ecrireOctets(const uint8_t* src, size_t size) {
    for (size_t i = 0; i < size; i++) {
        memoire[positionEcriture] = src[i];
        positionEcriture = (positionEcriture + 1) % taille_memoire;
    }
    octetsOccupes += size;
}


// Copie size octets du tampon a partir de positionLecture, en revenant au debut si besoin
static void lireOctets(uint8_t* dst, size_t size) {
    for (size_t i = 0; i < size; i++) {
        dst[i] = memoire[positionLecture];
        positionLecture = (positionLecture + 1) % taille_memoire;
    }
    octetsOccupes -= size;
}


int allocateMemory(size_t size) {
    pthread_mutex_lock(&mutex_memoire);
    memoire = malloc(size);
    taille_memoire = (memoire == NULL) ? 0 : size;
    positionLecture = 0;
    positionEcriture = 0;
    octetsOccupes = 0;
    pthread_mutex_unlock(&mutex_memoire);
    return (memoire == NULL) ? -1 : 0;
}



// Renvoie -1 si le message ne tient pas dans la place libre : il est alors perdu
int writeMemory(void* data, uint16_t size) {
    int retour = -1;
    pthread_mutex_lock(&mutex_memoire);
    if (memoire != NULL && data != NULL && octetsOccupes + sizeof(size) + size <= taille_memoire) {
        ecrireOctets((const uint8_t*)&size, sizeof(size));
        ecrireOctets((const uint8_t*)data, size);
        pthread_cond_signal(&cond_memoire);
        retour = 0;
    }
    pthread_mutex_unlock(&mutex_memoire);
    return retour;
}



// Bloquant : attend qu'un message soit disponible puis le renvoie (a liberer avec free)
void* readMemory(uint16_t* size) {
    pthread_mutex_lock(&mutex_memoire);
    while (memoire == NULL || octetsOccupes == 0) {
        pthread_cond_wait(&cond_memoire, &mutex_memoire);
    }
    lireOctets((uint8_t*)size, sizeof(*size));
    uint8_t* data = malloc(*size);
    if (data != NULL) {
        lireOctets(data, *size);
    } else {
        // on saute le message pour garder le tampon coherent
        positionLecture = (positionLecture + *size) % taille_memoire;
        octetsOccupes -= *size;
        *size = 0;
    }
    pthread_mutex_unlock(&mutex_memoire);
    return (void*)data;
}
