#include "listFuncs.h"

lista_t *crtNode(const string_t* data)
{
    lista_t *node = malloc(sizeof(lista_t));
    if(!node)
    {
        fprintf(stderr, "Error solicitando memoria\n");
        return NULL;
    }

    node->data = *data;

    return node;
}

//NOTA: la lista tiene el modelo FIFO (First Input First Output)
int pushList(lista_t **begin, const string_t *data)
{
    lista_t *node = crtNode(data);
    if(!node)
    {
        fprintf(stderr, "Error creando nodo\n");
        return ERR;
    }

    node->data = *data;
    if(!begin)
    {
        node->next = NULL;
    }
    else{
        node->next = *begin;
    }
    *begin = node;

    return OK;
}