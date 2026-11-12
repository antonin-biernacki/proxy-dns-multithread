#ifndef LIBMEMOIRE_H
#define LIBMEMOIRE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <pthread.h>


int allocateMemory(size_t size);
int writeMemory(void *data, uint16_t size);
void *readMemory(uint16_t *size);


#endif
