#ifndef __FTHREADS_H
#define __FTHREADS_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

extern pthread_mutex_t myMutex;

#define OK 1
#define ERR 0

#define CAPACIDAD 32

#define FINALIZADOR "InfoI-2022"

#define N 64

typedef struct
{
    char **vecStr;
    int sz; //Osea la capacidad

    int contProductor; //Este va a ir añadiendo datos
    int contArc, contHisto; //Estos van a ir sacando datos
    int flagArc, flagHisto; //Flags para saber si se puede eliminar o no

    int flagFull; //Este me avisa si esta lleno o no

    int flagEnd; //Si es positivo, significa que el programa llego a su fin

    int *vecHisto;
}param_t;

void* inputThread(void*);

void* fileThread(void*);

void* histograma(void*);

#endif