#ifndef __FTHREADS_H
#define __FTHREADS_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

extern pthread_mutex_t myMutex;

typedef struct
{
    char **vecStr;
    int contProductor; //Este va a ir añadiendo datos
    int contArc, contHisto; //Estos van a ir sacando datos
    int flagArc, flagHisto; //Flags para saber si se puede eliminar o no

    int flagFull;
}param_t;

void* inputThread(void* param);

void* fileThread(void* param);

void* histograma(void* param);

#endif