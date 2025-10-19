#include "fThreads.h"
#include <unistd.h>
#include <fcntl.h>

int extPilaFile(param_t *p, char *auxRet);

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
        char strAnalyze[N];

        pthread_mutex_lock(&myMutex);

        while(!(extPilaFile(param, strAnalyze)))
        {
            pthread_cond_wait(&param->isEmpty, &myMutex);
        }
        pthread_cond_broadcast(&param->isFull);

        pthread_mutex_unlock(&myMutex);   
        
        if (!strcmp(strAnalyze, FINALIZADOR))
        {
            param->flagEnd = 0;
            pthread_cond_broadcast(&param->isEmpty);
            break;
        }

        int szS = strlen(strAnalyze);

        if(write(f, strAnalyze, szS) < 0)
        {
            printf("Error al escribir datos\n");
        }
    }

    close(f);
    pthread_exit(NULL);
}

int extPilaFile(param_t *p, char *auxRet)
{
    if (p->cantArc == 0)
    {
        return ERR;
    }

    char *auxStr = (p->vecStr[p->contHisto]).s;

    strcpy(auxRet, auxStr);

    (p->vecStr[p->contArc]).flagArc = 1;

    if((p->vecStr[p->contArc]).flagHisto)
    {
        free(auxStr);
        (p->vecStr[p->contArc]).s = NULL;
        (p->vecStr[p->contArc]).flagHisto = 0;
        (p->vecStr[p->contArc]).flagArc = 0;
    }
    p->contArc++;
    p->contArc %= p->sz;

    p->cantArc--;

    return OK;
}