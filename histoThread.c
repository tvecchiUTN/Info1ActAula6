#include "fThreads.h"
#include <string.h>

int extPilaHisto(param_t *p, char* data);

void calcHisto(const char* s, int *histo);

void* histograma(void* p)
{
    param_t *param = (param_t*)p;

    while(param->flagEnd)
    {
        char* strAnalyze = NULL;

        pthread_mutex_lock(&myMutex);

        while(1)
        {
            if(extPilaHisto(param, strAnalyze))
            {
                break;
            }
        }

        pthread_mutex_unlock(&myMutex);

        if(strcmp(strAnalyze, FINALIZADOR))
        {
            param->flagEnd = 0;
            free(strAnalyze);
            break;
        }
        
        calcHisto(strAnalyze, param->vecHisto);

        pthread_mutex_lock(&myMutex);

        param->flagHisto = 1;

        if(param->flagHisto && param->flagArc)
        {
            free(strAnalyze);
            param->flagArc = 0;
            param->flagHisto = 0;
        }

        pthread_mutex_unlock(&myMutex);
    }

    pthread_exit(NULL);
}

int extPilaHisto(param_t *p, char* data)
{
    if((p->contProductor == p->contArc) && (p->contProductor == p->contHisto) && !p->flagFull)
    {
        return ERR;
    }

    data = p->vecStr[p->contHisto];
    p->contHisto++;
    p->contHisto %= p->sz;

    p->flagFull = 0;

    return OK;
}

void calcHisto(const char* s, int *histo)
{
    for(int i = 0; s[i]; i++)
    {
        if((s[i] >= 'a') && (s[i] <= 'z'))
        {
            int n = s[i] - 'a';
            histo[n]++;
        }
        else if((s[i] >= 'A') && (s[i] <= 'Z'))
        {
            int n = s[i] - 'A';
            histo[n]++;
        }
    }
}