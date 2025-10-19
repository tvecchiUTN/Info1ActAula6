#include "listFuncs.h"

void *inputThread(void* param)
{
    param_t *p = (param_t*)param;

    while(p->flagEnd)
    {
        char *str = malloc(SZSTR);
        if(!str)
        {
            printf("Error solicitando memoria\n");
            continue;
        }

        printf("Introduzca el texto: ");
        fgets(str, SZSTR, stdin);

        string_t auxStr;

        auxStr.str = str;
        auxStr.flagHisto = 0;
        auxStr.flagFile = 0;

        pthread_mutex_lock(&myMutex);

        pushList(&p->initList, &auxStr);

        pthread_mutex_unlock(&myMutex);

        if(!strcmp(str, FINALIZADOR))
        {
            break;
        }
    }

    pthread_exit(NULL);

    return NULL;
}
