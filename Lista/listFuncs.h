#ifndef __LISTFUNCS_H
#define __LISTFUNCS_H

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

extern pthread_mutex_t myMutex;

#define ERR 0
#define OK 1

#define FINALIZADOR "InfoI-2022\n"

void *inputThread(void* param);

void* histoThread(void* param);

void* fileThread(void* param);

#endif