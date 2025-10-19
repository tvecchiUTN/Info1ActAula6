#ifndef __LISTFUNCS_H
#define __LISTFUNCS_H

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

#define SZSTR 32

typedef struct
{
    char *str;
    unsigned int flagHisto, flagFile;
}string_t;

typedef struct lista
{
    string_t data;

    struct lista *next;
}lista_t;

typedef struct
{
    lista_t *initList;
    unsigned int flagEnd;
    int *histograma;
}param_t;

extern pthread_mutex_t myMutex;

#define ERR 0
#define OK 1

#define FINALIZADOR "InfoI-2022\n"

int pushList(lista_t **begin, const string_t *data);

int popData(lista_t *begin, string_t *data);

int popList(lista_t **begin);

void *inputThread(void* param);

void* histoThread(void* param);

void* fileThread(void* param);

#endif