#include "listFuncs.h"

static int comparador(string_t* cmp);

void histograma(int *histo, const char* s);

void* histoThread(void* param)
{
    param_t *p = (param_t*)param;

    while(p->flagEnd)
    {
        char strRx[SZSTR];

        string_t strData;

        pthread_mutex_lock(&myMutex);

        if(!popData(p->initList, &strData, comparador, 0))
        {
            pthread_mutex_unlock(&myMutex);
            sleep(1);
            continue;
        }

        strcpy(strRx, strData.str);

        pthread_mutex_unlock(&myMutex);

        if(!strcmp(strRx, FINALIZADOR))
        {
            break;
        }

        histograma(p->histograma, strRx);

        if(SHOWHISTO)
        {
            for(int i = 0; i < 26; i++)
            {
                printf("La letra %c aparecio %d\n", 'A'+i, p->histograma[i]);
            }
            SHOWHISTO = 0;
        }
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

static int comparador(string_t* cmp)
{
    if(cmp->flagHisto)
    {
        return NO;
    }

    return SI;
}