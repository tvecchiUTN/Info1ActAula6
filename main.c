#include <stdio.h>
#include <stdlib.h>
#include "fThreads.h"

pthread_mutex_t myMutex;

// Falta añadir el tema de las señales

int initParam(param_t *param);

int main()
{
    pthread_t hiloInput, hiloFile, hiloHisto;

    param_t *paramSend = malloc(sizeof(param_t));
    if (!paramSend)
    {
        printf("Error soliciendo memoria\n");
        return 1;
    }

    if(!initParam(paramSend))
    {
        return 1;
    }

    pthread_mutex_init(&myMutex, NULL);

    int err;
    err = pthread_create(&hiloInput, NULL, inputThread, paramSend);
    if (err)
    {
        printf("Error creando hilo para entrada de datos\n");

        free(paramSend);
        return 1;
    }

    err = pthread_create(&hiloHisto, NULL, histograma, paramSend);
    if (err)
    {
        printf("Error creando hilo para el histograma\n");
    }

    err = pthread_create(&hiloFile, NULL, fileThread, paramSend);
    if (err)
    {
        printf("Error creando hilo que guarda datos en el archivo\n");
    }

    pthread_exit(NULL);

    pthread_mutex_destroy(&myMutex);

    free(paramSend->vecStr);
    free(paramSend);
}

int initParam(param_t *param)
{
    param->vecStr = calloc(CAPACIDAD, sizeof(char*));
    if (!param->vecStr)
    {
        printf("Error soliciendo memoria\n");
        free(param);
        return ERR;
    }

    param->sz = CAPACIDAD;
    param->contArc = 0;
    param->contHisto = 0;
    param->contProductor = 0;
    param->flagArc = 0;
    param->flagHisto = 0;
    param->flagFull = 0;

    param->flagEnd = 1;

    param->vecHisto = calloc(26, sizeof(int));
    if(!param->vecHisto)
    {
        printf("Error soliciendo memoria\n");
        free(param);
        return ERR;
    }

    return OK;
}