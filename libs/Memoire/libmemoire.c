#include "libmemoire.h"

/*
 * Memoire partagee entre les threads de requete et le thread de log.
 * Chaque message est stocke precede de sa taille (2 octets) pour que le lecteur
 * recupere les messages un par un, meme si plusieurs se sont accumules.
 */

uint8_t* memoire = NULL;
size_t taille_memoire = 0;                                  // taille de la memoire
size_t positionLecture = 0;                                 // premier octet pas encore lu
size_t positionEcriture = 0;                                // premier octet libre
pthread_mutex_t mutex_memoire = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_memoire = PTHREAD_COND_INITIALIZER;     // signale qu'un message est disponible


int allocateMemory(size_t size) {
    taille_memoire = size;
    memoire = malloc(size);
    if (memoire == NULL) {
        return -1;
    }
    return 0;
}



int writeMemory(void* data, uint16_t size) {
    pthread_mutex_lock(&mutex_memoire);
    if (memoire != NULL && data != NULL && (positionEcriture + sizeof(size) + size) <= taille_memoire) {
        memcpy(memoire + positionEcriture, &size, sizeof(size));
        positionEcriture += sizeof(size);
        memcpy(memoire + positionEcriture, data, size);
        positionEcriture += size;
        pthread_cond_signal(&cond_memoire);
        pthread_mutex_unlock(&mutex_memoire);
        return 0;
    }
    pthread_mutex_unlock(&mutex_memoire);
    return -1;
}



// Bloquant : attend qu'un message soit disponible puis le renvoie (a liberer avec free)
void* readMemory(uint16_t* size) {
    pthread_mutex_lock(&mutex_memoire);
    while (memoire == NULL || positionLecture == positionEcriture) {
        pthread_cond_wait(&cond_memoire, &mutex_memoire);
    }
    memcpy(size, memoire + positionLecture, sizeof(*size));
    positionLecture += sizeof(*size);
    uint8_t* data = malloc(*size);
    if (data != NULL) {
        memcpy(data, memoire + positionLecture, *size);
    }
    positionLecture += *size;
    if (positionLecture == positionEcriture) {   // si la lecture a rattrapé l'écriture alors elle "libère" la mémoire
        positionEcriture = 0;
        positionLecture = 0;
    }
    pthread_mutex_unlock(&mutex_memoire);
    if (data == NULL) {
        *size = 0;
    }
    return (void*)data;
}
