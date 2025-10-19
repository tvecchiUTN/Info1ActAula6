#include "listFuncs.h"

void *inputThread(void* param)
{
    param_t *p = (param_t*)param;

    while(p->flagEnd)
    {
        char str[SZSTR];

        fgets(str, SZSTR, stdin);

        string_t *auxStr = malloc(sizeof(string_t));
        if(!auxStr)
        {
            printf("Error solicitando memoria\n");
            continue;
        }

        strcpy(auxStr->str, str);

        pthread_mutex_lock(&myMutex);

        pushList(&p->initList, auxStr);

        pthread_mutex_unlock(&myMutex);

        free(auxStr);
    }
}
