#include "fThreads.h"
#include <stdlib.h>

int addPila(param_t *param, char* str);

void *inputThread(void *p)
{
    param_t *parametros = (param_t *)p;

    while (parametros->flagEnd)
    {
        char *buffStr = malloc(N); // Sizeof(char) es 1
        if (!buffStr)
        {
            printf("Error solicitando memoria\n");
            parametros->flagEnd = 1; //De este motivo, los otros hilos tambien cortan
            break;
        }

        printf("Introduzca el texto: ");
        fgets(buffStr, N, stdin);

        pthread_mutex_lock(&myMutex);

        while(!addPila(parametros, buffStr))
        {
            pthread_cond_wait(&parametros->isFull, &myMutex);
        }
        pthread_cond_broadcast(&parametros->isEmpty);

        if(!strcmp(buffStr, FINALIZADOR))
        {
            parametros->flagEnd = 0;
            pthread_cond_broadcast(&parametros->isEmpty);
            pthread_mutex_unlock(&myMutex);    
            break;
        }

        pthread_mutex_unlock(&myMutex);
    }

    pthread_exit(NULL);

    return NULL;
}

int addPila(param_t *param, char* str)
{
    if (param->cantArc >= param->sz || param->cantHisto >= param->sz)
    {
        return ERR;
    }

    int auxFArc = (param->vecStr[param->contProductor]).flagArc;
    int auxFHis = (param->vecStr[param->contProductor]).flagHisto;
    if(auxFArc && auxFHis)
    {
        free((param->vecStr[param->contProductor]).s);
        (param->vecStr[param->contProductor]).s = NULL;
        (param->vecStr[param->contProductor]).flagArc = 0;
        (param->vecStr[param->contProductor]).flagHisto = 0;
    }

    (param->vecStr[param->contProductor]).s = str;

    param->contProductor++;

    param->contProductor %= param->sz;
    
    param->cantArc++;
    param->cantHisto++;

    return OK;
}