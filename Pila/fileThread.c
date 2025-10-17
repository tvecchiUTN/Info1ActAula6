#include "fThreads.h"
#include <unistd.h>
#include <fcntl.h>

char *extPilaFile(param_t *p);

void *fileThread(void *p)
{
    param_t *param = (param_t *)p;

    int f = open("histograma.txt", O_WRONLY | O_CREAT | O_TRUNC, 0664);
    if (f < 0)
    {
        printf("EL archivo no se pudo crear\n");
        pthread_exit(NULL);
    }

    while (param->flagEnd)
    {
        char *strAnalyze = NULL;
        char cpyStr[N];

        pthread_mutex_lock(&myMutex);

        while(!(strAnalyze = extPilaFile(param)))
        {
            pthread_cond_wait(&param->isEmpty, &myMutex);
        }
        pthread_cond_broadcast(&param->isFull);

        strcpy(cpyStr, strAnalyze);

        pthread_mutex_unlock(&myMutex);   
        
        if (!strcmp(cpyStr, FINALIZADOR))
        {
            param->flagEnd = 0;
            pthread_cond_broadcast(&param->isEmpty);
            break;
        }

        int szS = strlen(cpyStr);

        if(write(f, cpyStr, szS) < 0)
        {
            printf("Error al escribir datos\n");
        }
    }

    close(f);
    pthread_exit(NULL);
}

char *extPilaFile(param_t *p)
{
    if (p->cantArc == 0)
    {
        return NULL;
    }

    char *auxRet = (p->vecStr[p->contArc]).s;
    (p->vecStr[p->contArc]).flagArc = 1;

    p->contArc++;
    p->contArc %= p->sz;

    p->cantArc--;

    return auxRet;
}