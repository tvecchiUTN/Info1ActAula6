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
        char copyStr[N];

        pthread_mutex_lock(&myMutex);

        while(!(strAnalyze = extPilaHisto(param)))
        {
            pthread_cond_wait(&param->isEmpty, &myMutex);
        }
        pthread_cond_broadcast(&param->isFull);

        strcpy(copyStr, strAnalyze);

        pthread_mutex_unlock(&myMutex);

        if (!strcmp(copyStr, FINALIZADOR))
        {
            param->flagEnd = 0;
            pthread_cond_broadcast(&param->isEmpty);
            break;
        }

        calcHisto(copyStr, param->vecHisto);

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

char *extPilaHisto(param_t *p)
{
    if (p->cantHisto == 0)
    {
        return NULL;
    }

    char *auxRet = (p->vecStr[p->contHisto]).s;
    (p->vecStr[p->contHisto]).flagHisto = 1;

    p->contHisto++;
    p->contHisto %= p->sz;

    p->cantHisto--;

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