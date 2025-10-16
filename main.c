#include <stdio.h>
#include "fThreads.h"

//Falta añadir el tema de las señales

int main()
{
    pthread_t hiloInput, hiloFile, hiloHisto;
    
    param_t *paramSend = malloc(sizeof(param_t));
    if(!paramSend)
    {
        printf("Error soliciendo memoria\n");
        return 1;
    }

    paramSend ->contArc = 0;
    paramSend ->contHisto = 0;
    paramSend ->contProductor = 0;
    paramSend ->flagArc = 0;
    paramSend ->flagHisto = 0;
    paramSend ->flagFull = 0;

    int err;
    err = pthread_create(hiloInput, NULL, inputThread, paramSend);
    if(err)
    {
        printf("Error creando hilo para entrada de datos\n");

        free(paramSend);
        return 1;
    }

    err = pthread_create(hiloHisto, NULL, histograma, paramSend);
    if(err)
    {
        printf("Error creando hilo para el histograma\n");
    }

    err = pthread_create(hiloFile, NULL, fileThread, paramSend);
    if(err)
    {
        printf("Error creando hilo que guarda datos en el archivo\n");
    }

    pthread_exit(NULL);

    free(paramSend);
}