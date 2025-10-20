#include "listFuncs.h"

lista_t *crtNode(const string_t *data)
{
    lista_t *node = malloc(sizeof(lista_t));
    if (!node)
    {
        fprintf(stderr, "Error solicitando memoria\n");
        return NULL;
    }

    node->data = *data;
    node->next = NULL;

    return node;
}

// NOTA: la lista tiene el modelo FIFO (First Input First Output)
int pushList(lista_t **begin, const string_t *data)
{
    lista_t *node = crtNode(data);
    if (!node)
    {
        // fprintf(stderr, "Error creando nodo\n");
        return ERR;
    }

    if (!*begin)
    {
        *begin = node;
    }
    else
    {
        lista_t *auxInit = NULL;
        for (auxInit = *begin; auxInit->next; auxInit = auxInit->next)
            ;
        auxInit->next = node;
    }
    return OK;
}

int popData(lista_t *begin, string_t *data, int (*cond)(string_t *), int mode)
{
    while(begin)
    {
        if (cond(&begin->data))
        {
            *data = begin->data;

            if(!mode) //Histo tiene modo 0
            {
                begin->data.flagHisto = 1;
            }
            else{ //File tiene cualquier otro
                begin->data.flagFile = 1;
            }
            return OK;
        }

        begin = begin->next;
    }

    return ERR;
}

int popList(lista_t **begin)
{
    if (!begin)
    {
        return ERR;
    }

    lista_t *auxInit = (*begin)->next;
    free(*begin);

    *begin = auxInit;

    return OK;
}