#include "fThreads.h"
#include <stdlib.h>

pthread_mutex_t myMutex;

#define N 64
void* inputThread(void* param)
{
    param_t *parametros = (param_t *)param;

    while(1)
    {
        char *buffStr = malloc(N); //Sizeof(char) es 1
        if(!buffStr)
        {
            printf("Error solicitando memoria\n");
            break;
        }

        fgets(buffStr, N, stdin);

        pthread_mutex_lock(&myMutex);
        parametros ->vecStr[parametros ->contProductor] = buffStr;

        parametros->contProductor += 1;

        parametros->flagFull = 1;

        pthread_mutex_unlock(&myMutex);
    }
    
    pthread_exit(NULL);

    return NULL;
}