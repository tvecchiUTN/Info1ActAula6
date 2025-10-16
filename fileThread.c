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

        pthread_mutex_lock(&myMutex);

        strAnalyze = extPilaFile(param);
        if (!strAnalyze)
        {
            pthread_cond_wait(&param->isEmpty, &myMutex);
            strAnalyze = extPilaFile(param);
        }
        pthread_cond_signal(&param->isFull);

        pthread_mutex_unlock(&myMutex);

        char auxS[N];
        int i;

        pthread_mutex_lock(&myMutex);
        for (i = 0; (strAnalyze[i] != '\n') && strAnalyze[i]; i++)
        {
            auxS[i] = strAnalyze[i];
        }

        auxS[i] = '\n';

        if (write(f, auxS, i + 1) < 0)
        {
            printf("Error al escribir datos\n");
        }
        pthread_mutex_unlock(&myMutex);
    }

    close(f);
    pthread_exit(NULL);
}

char *extPilaFile(param_t *p)
{
    if ((p->contProductor == p->contArc) && (p->contProductor == p->contHisto) && !p->flagFull)
    {
        return NULL;
    }

    char *auxRet = (p->vecStr[p->contArc]).s;
    (p->vecStr[p->contArc]).flagArc = 1;

    p->contArc++;
    p->contArc %= p->sz;

    p->flagFull = 0;

    return auxRet;
}