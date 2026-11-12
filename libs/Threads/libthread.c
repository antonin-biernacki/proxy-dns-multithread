#include "libthread.h"

// Copie les arguments dans le tas (le thread les libere) puis lance un thread detache
int launchThread(void* (*fonction)(void *), void *args, size_t argsSize) {
    pthread_t tid;
    void* args_thread = malloc(argsSize);
    if (args_thread == NULL) {
        return -1;
    }
    memcpy(args_thread, args, argsSize);
    if (pthread_create(&tid, NULL, fonction, args_thread) != 0) {
        free(args_thread);
        return -1;
    }
    pthread_detach(tid);
    return 0;
}
