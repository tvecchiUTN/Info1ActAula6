#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "fThreads.h"

pthread_mutex_t myMutex;

// Falta añadir el tema de las señales

volatile sig_atomic_t FLAGHISTO = 0;

void sigHisto(int sig)
{
    FLAGHISTO = 1;
}

int initParam(param_t *param);

int main()
{
    pthread_t hiloInput, hiloFile, hiloHisto;

    signal(SIGUSR1, sigHisto);
    
    printf("%d\n", getpid());

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

    pthread_join(hiloInput, NULL);
    pthread_join(hiloFile, NULL);
    pthread_join(hiloHisto, NULL);

    pthread_mutex_destroy(&myMutex);

    pthread_cond_destroy(&paramSend->isFull);
    pthread_cond_destroy(&paramSend->isEmpty);

    int reAux;
    if(!paramSend->contProductor)
    {
        reAux = paramSend->sz-1;
    }else
    {
        reAux = paramSend->contProductor--;
    }
    free((paramSend->vecStr[reAux]).s);
    free(paramSend->vecStr);
    free(paramSend->vecHisto);
    free(paramSend);

    return 0;
}

int initParam(param_t *param)
{
    param->vecStr = calloc(CAPACIDAD, sizeof(string_t));
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
    
    param->cantArc = 0;
    param->cantHisto = 0;

    param->flagEnd = 1;

    param->vecHisto = calloc(26, sizeof(int));
    if(!param->vecHisto)
    {
        printf("Error soliciendo memoria\n");
        free(param);
        return ERR;
    }

    pthread_cond_init(&param->isFull, NULL);
    pthread_cond_init(&param->isEmpty, NULL);

    return OK;
}