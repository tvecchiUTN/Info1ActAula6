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

        while(!(strAnalyze = extPilaHisto(param)))
        {
            pthread_cond_wait(&param->isEmpty, &myMutex);
        }
        pthread_cond_broadcast(&param->isFull);

        pthread_mutex_unlock(&myMutex);

        if (!strcmp(strAnalyze, FINALIZADOR))
        {
            param->flagEnd = 0;
            free(strAnalyze);
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

        free(strAnalyze);
    }

    pthread_exit(NULL);
}

char *extPilaHisto(param_t *p)
{
    if (p->cantHisto == 0)
    {
        return NULL;
    }

    char *auxStr = (p->vecStr[p->contHisto]).s;

    char *auxRet = malloc(N);
    if(!auxRet)
    {
        printf("Error al solicitar memoria\n");
        return NULL;
    }

    strcpy(auxRet, auxStr);

    (p->vecStr[p->contHisto]).flagHisto = 1;

    if((p->vecStr[p->contArc]).flagArc)
    {
        free(auxStr);
        (p->vecStr[p->contArc]).s = NULL;
        (p->vecStr[p->contArc]).flagHisto = 0;
        (p->vecStr[p->contArc]).flagArc = 0;
    }
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