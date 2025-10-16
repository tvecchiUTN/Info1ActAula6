#include "fThreads.h"
#include <string.h>

char *extPilaHisto(param_t *p);

void calcHisto(const char *s, int *histo);

void *histograma(void *p)
{
    param_t *param = (param_t *)p;

    while (param->flagEnd)
    {
        char *strAnalyze = NULL;

        pthread_mutex_lock(&myMutex);

        strAnalyze = extPilaHisto(param);
        if (!strAnalyze)
        {
            pthread_cond_wait(&param->isEmpty, &myMutex);
            strAnalyze = extPilaHisto(param);
        }
        pthread_cond_signal(&param->isFull);

        if (!strcmp(strAnalyze, FINALIZADOR))
        {
            param->flagEnd = 0;
            free(strAnalyze);
            break;
        }

        calcHisto(strAnalyze, param->vecHisto);

        pthread_mutex_unlock(&myMutex);
    }

    pthread_exit(NULL);
}

char *extPilaHisto(param_t *p)
{
    if ((p->contProductor == p->contArc) && (p->contProductor == p->contHisto) && !p->flagFull)
    {
        return NULL;
    }

    char *auxRet = (p->vecStr[p->contHisto]).s;
    (p->vecStr[p->contHisto]).flagHisto = 1;

    p->contHisto++;
    p->contHisto %= p->sz;

    p->flagFull = 0;

    return auxRet;
}

void calcHisto(const char *s, int *histo)
{
    for (int i = 0; s[i]; i++)
    {
        if ((s[i] >= 'a') && (s[i] <= 'z'))
        {
            int n = s[i] - 'a';
            histo[n]++;
        }
        else if ((s[i] >= 'A') && (s[i] <= 'Z'))
        {
            int n = s[i] - 'A';
            histo[n]++;
        }
    }
}