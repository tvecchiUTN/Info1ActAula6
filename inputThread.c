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

        if(!addPila(parametros, buffStr))
        {
            pthread_cond_wait(&parametros->isFull, &myMutex);
            addPila(parametros, buffStr);
        }
        pthread_cond_broadcast(&parametros->isEmpty);

        pthread_mutex_unlock(&myMutex);    
    }

    pthread_exit(NULL);

    return NULL;
}

int addPila(param_t *param, char* str)
{
    if((param->contProductor == param->contArc) && (param->contProductor == param->contHisto) && param->flagFull)
    {
        return ERR;
    }

    (param->vecStr[param->contProductor]).s = str;

    param->contProductor++;

    param->contProductor %= param->sz;

    param->flagFull = 1;

    return OK;
}