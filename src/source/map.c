#include "../include/map.h"
#include <stdio.h>

map_t * map_create()
{
    map_t *t = malloc(sizeof(map_t));
    t->key = NULL;
    t->value = NULL;
    t->next = NULL;

    return t;
}

bool map_add(map_t *map ,char * key,char *value)
{
    
}