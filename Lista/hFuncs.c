#include "listFuncs.h"

void histograma(int *histo, const char* s);

void* histoThread(void* param)
{
    param_t *p = (param_t*)param;

    while(p->flagEnd)
    {
        char strRx[SZSTR];

        string_t strData;

        pthread_mutex_lock(&myMutex);

        if(!popData(p->initList, &strData))
        {
            pthread_mutex_unlock(&myMutex);
            continue;
        }

        strcpy(strRx, strData.str);

        p->initList->data.flagHisto = 1;

        pthread_mutex_unlock(&myMutex);

        if(!strcmp(strRx, FINALIZADOR))
        {
            break;
        }

        histograma(p->histograma, strRx);
    }

    for(int i = 0; i < 26; i++)
    {
        printf("La letra %c aparecio %d\n", 'A' + i, p->histograma[i]);
    }

    pthread_exit(NULL);

    return NULL;
}

void histograma(int *histo, const char* s)
{
    for(int i = 0; s[i]; i++)
    {
        if((s[i] >= 'a') && (s[i] <= 'z'))
        {
            int c = s[i] - 'a';
            histo[c]++;
        }
        else if((s[i] >= 'A') && (s[i] <= 'Z'))
        {
            int c = s[i] - 'A';
            histo[c]++;
        }
    }
}