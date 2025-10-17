#ifndef __FTHREADS_H
#define __FTHREADS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>

extern volatile sig_atomic_t FLAGHISTO;

extern pthread_mutex_t myMutex;

#define OK 1
#define ERR 0

#define CAPACIDAD 32 //Del array de strings

#define FINALIZADOR "InfoI-2022\n"

#define N 64 //Para el string

typedef struct 
{
    char* s;
    int flagArc, flagHisto;
}string_t;
typedef struct
{
    string_t *vecStr;
    
    int sz; //Osea la capacidad

    int contProductor; //Este va a ir añadiendo datos
    int contArc, contHisto; //Estos van a ir sacando datos

    int cantArc, cantHisto;

    int flagEnd; //Si es positivo, significa que el programa llego a su fin

    int *vecHisto;

    pthread_cond_t isFull, isEmpty;
}param_t;

void* inputThread(void*);

void* fileThread(void*);

void* histograma(void*);

#endif