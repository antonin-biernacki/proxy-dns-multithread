#ifndef GENERICLOG_H
#define GENERICLOG_H

#include <stdint.h>
#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>

#include "log_dns.h"
#include "../Memoire/libmemoire.h"

#define TAILLE 512


typedef struct {
    char strategie[TAILLE];
    char fichier_logs[TAILLE];
} argsLog_t;


void* loadStrategy(void*);
int initStrategy(void *);
int logStrategy(logMsg_t *);


#endif
