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
    if(map == NULL)
        return false;
    
    map_t *tmp = map;
    map_t *new_map = map_create();
    while(tmp->next != NULL)
    {
        tmp = tmp->next;
    }

    fill_map_item(new_map,key,value);

    tmp->next = new_map;
}

void map_print(map_t *map)
{
    map_t *tmp = map;
    while(tmp != NULL)
    {
        printf("%s : %s\n",tmp->key,tmp->value);
    }
}

void fill_map_item(map_t *map,char *key,char *value)
{
    int vlen = strlen(value)+1;
    int klen = strlen(key)+1;
    string_t *strtmp = string_create_from_string(key);

    map->value = malloc(vlen * sizeof(char));
    map->key = malloc(klen * sizeof(char));
    strcpy(map->key,strtmp->chars);
    map->value[vlen] = '\0';
    string_destroy(strtmp);
    strtmp = string_create_from_string(value);
    map->key[klen] = '\0';
    strcpy(map->value,strtmp->chars);

    map->next = NULL;
}