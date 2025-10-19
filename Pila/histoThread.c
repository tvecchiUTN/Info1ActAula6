#include "fThreads.h"
#include <string.h>

int extPilaHisto(param_t *p, char *auxRet);

void calcHisto(const char *s, int *histo);

void *histograma(void *p)
{
    param_t *param = (param_t *)p;

    while (param->flagEnd)
    {
        char strAnalyze[N];

        pthread_mutex_lock(&myMutex);

        while(!(extPilaHisto(param, strAnalyze)))
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

        calcHisto(strAnalyze, param->vecHisto);

        if(FLAGHISTO)
        {
            for(int i = 0; i < 26; i++)
            {
                printf("La letra %c aparecio %d\n", 'a'+i, param->vecHisto[i]);
            }
            FLAGHISTO = 0;
        }
    }

    pthread_exit(NULL);
}

int extPilaHisto(param_t *p, char *auxRet)
{
    if (p->cantHisto == 0)
    {
        return ERR;
    }

    char *auxStr = (p->vecStr[p->contHisto]).s;

    strcpy(auxRet, auxStr);

    (p->vecStr[p->contHisto]).flagHisto = 1;

    if((p->vecStr[p->contHisto]).flagArc)
    {
        free(auxStr);
        (p->vecStr[p->contArc]).s = NULL;
        (p->vecStr[p->contArc]).flagHisto = 0;
        (p->vecStr[p->contArc]).flagArc = 0;
    }
    p->contHisto++;
    p->contHisto %= p->sz;

    p->cantHisto--;

    return OK;
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