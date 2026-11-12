#ifndef LOG_DNS_H
#define LOG_DNS_H

#include <stdint.h>
#include <stdio.h>


typedef struct {
	uint8_t* octets;
	uint16_t longueur;
} logMsg_t;


int initLog(void *);
int addLog(logMsg_t *);


#endif
