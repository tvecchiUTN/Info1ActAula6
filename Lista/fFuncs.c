#include "listFuncs.h"

#include <unistd.h>
#include <fcntl.h>

static int comparador(string_t* cmp);

void *fileThread(void *param)
{
    param_t *p = (param_t *)param;

    int f = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0664);
    if (f < 0)
    {
        printf("Error creando el archivo\n");
        return NULL;
    }

    while (p->flagEnd)
    {
        char strRx[SZSTR];

        string_t strData;

        pthread_mutex_lock(&myMutex);

        if(!p->initList || !p->initList->data.flagHisto)
        {
            pthread_mutex_unlock(&myMutex);
            sleep(1);
            continue;
        }

        popData(p->initList, &strData, comparador, 1);
        strcpy(strRx, strData.str);

        free(p->initList->data.str);
        popList(&p->initList);

        pthread_mutex_unlock(&myMutex);

        if(!strcmp(strRx, FINALIZADOR))
        {
            break;
        }

        int szStr = strlen(strRx);

        if (write(f, strRx, szStr) < 0)
        {
            printf("Error escribiendo el el archivo\n");
        }
    }

    close(f);

    pthread_exit(NULL);

    return NULL;
}

static int comparador(string_t* cmp)
{
    if(cmp->flagFile)
    {
        return NO;
    }

    return SI;
}