#include "fThreads.h"
#include <unistd.h>
#include <fcntl.h>

pthread_mutex_t myMutex;

int extPilaFile(param_t *p, char *data);

void *fileThread(void *p)
{
    param_t *param = (param_t *)p;

    while (param->flagEnd)
    {
        char *strAnalyze = NULL;

        int f = open("histograma.txt", O_WRONLY | O_CREAT | O_TRUNC, 0664);
        if (f < 0)
        {
            printf("EL archivo no se pudo crear\n");
            param->flagArc = 1;
            break;
        }

        pthread_mutex_lock(&myMutex);

        while (1)
        {
            if (extPilaFile(param, strAnalyze))
            {
                break;
            }
        }

        pthread_mutex_unlock(&myMutex);

        if(write(f, strAnalyze, N) < 0)
        {
            printf("Error al escribir datos\n");
        } 

        pthread_mutex_lock(&myMutex);

        param->flagArc = 1;

        if(param->flagArc && param->flagHisto)
        {
            free(strAnalyze);
            param->flagArc = 0;
        }

        pthread_mutex_unlock(&myMutex);
    }

    pthread_exit(NULL);
}

int extPilaFile(param_t *p, char *data)
{
    if ((p->contProductor == p->contArc) && (p->contProductor == p->contHisto) && !p->flagFull)
    {
        return ERR;
    }

    data = p->vecStr[p->contArc];
    p->contArc++;
    p->contArc %= p->sz;

    p->flagFull = 0;

    return OK;
}