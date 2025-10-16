#include "fThreads.h"
#include <stdlib.h>

pthread_mutex_t myMutex;

int addPila(param_t *p, const char* s);

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

        fgets(buffStr, N, stdin);

        pthread_mutex_lock(&myMutex);

        while(1)
        {
            if(addPila(parametros, buffStr))
            {
                break;
            }
        }

        pthread_mutex_unlock(&myMutex);    
    }

    pthread_exit(NULL);

    return NULL;
}

int addPila(param_t *param, const char* s)
{
    if((param->contProductor == param->contArc) && (param->contProductor == param->contHisto) && param->flagFull)
    {
        return ERR;
    }

    param->vecStr[param->contProductor] = s;

    param->contProductor++;

    param->contProductor %= param->sz;

    param->flagFull = 1;

    return OK;
}