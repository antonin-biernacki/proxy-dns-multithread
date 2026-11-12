#ifndef LIBTHREAD_H
#define LIBTHREAD_H

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int launchThread(void *(*fonction)(void *), void *args, size_t argsSize);

#endif
