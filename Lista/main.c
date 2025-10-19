#include <stdio.h>
#include "listFuncs.h"

pthread_mutex_t myMutex;

int main()
{
    pthread_t hiloInput, hiloHisto, hiloFile;

    pthread_mutex_init(&myMutex, NULL);

    int err;

    err = pthread_create(&hiloInput, NULL, inputThread, NULL);
    if(err)
    {
        fprintf(stderr, "Error creando hilo para la entrada de datos\n");
        return ERR;
    }

    err = pthread_create(&hiloHisto, NULL, histoThread, NULL);
    if(err)
    {
        fprintf(stderr, "Error creando hilo histograma\n");
    }

    err = pthread_create(&hiloFile, NULL, fileThread, NULL);
    if(err)
    {
        fprintf(stderr, "Error creando hilo manejrador de archivos\n");
    }

    pthread_join(hiloInput, NULL);
    pthread_join(hiloHisto, NULL);
    pthread_join(hiloFile, NULL);

    pthread_mutex_destroy(&myMutex);

    return OK;
}