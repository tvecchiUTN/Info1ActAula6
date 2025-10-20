#include <stdio.h>
#include "listFuncs.h"

pthread_mutex_t myMutex;

volatile int SHOWHISTO = 0;

int main()
{
    signal(SIGUSR1, prtHisto);

    pthread_t hiloInput, hiloHisto, hiloFile;

    pthread_mutex_init(&myMutex, NULL);

    param_t *pSend = malloc(sizeof(param_t));
    if(!pSend)
    {
        printf("Error solicitando memoria para los parametros\n");
        return ERR;
    }

    pSend->initList = NULL;
    pSend->flagEnd = 1;
    pSend->histograma = calloc(26, sizeof(int));
    if(!pSend->histograma)
    {
        printf("Error solicitando memoria para el histograma");
    }

    int err;

    err = pthread_create(&hiloInput, NULL, inputThread, pSend);
    if(err)
    {
        fprintf(stderr, "Error creando hilo para la entrada de datos\n");
        return ERR;
    }

    err = pthread_create(&hiloHisto, NULL, histoThread, pSend);
    if(err)
    {
        fprintf(stderr, "Error creando hilo histograma\n");
    }

    err = pthread_create(&hiloFile, NULL, fileThread, pSend);
    if(err)
    {
        fprintf(stderr, "Error creando hilo manejador de archivos\n");
    }

    pthread_join(hiloInput, NULL);
    pthread_join(hiloHisto, NULL);
    pthread_join(hiloFile, NULL);

    pthread_mutex_destroy(&myMutex);

    free(pSend->histograma);
    free(pSend);

    return OK;
}

void prtHisto(int sig)
{
    SHOWHISTO = 1;
}