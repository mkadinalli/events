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

    if(map_len(map) == 0)
    {
        fill_map_item(map,key,value);
        return true;
    }
    
    map_t *tmp = map;
    map_t *new_map = map_create();
    while(tmp->next != NULL)
    {
        if(strcmp(tmp->key,key) == 0)
        {
            printf("Key exists.\n");
            new_map = NULL;
            return false;
        }

        tmp = tmp->next;
    }

    if(strcmp(tmp->key,key) == 0)
    {
        printf("Key exists.\n");
        new_map = NULL;
        return false;
    }

    fill_map_item(new_map,key,value);

    tmp->next = new_map;
    return true;
}

int map_len(map_t *list)
{
    if(list == NULL)
        return -1;

    int size = 0;
    map_t * temp_list = list;
    while(temp_list->next != NULL)
    {
        temp_list = temp_list->next;
        size++;
    }

    if(temp_list->value != NULL)
        size++;

    return size;
}

void map_print(map_t *map)
{
    map_t *tmp = map;
    while(tmp != NULL)
    {
        printf("%s : %s\n",tmp->key,tmp->value);
        tmp = tmp->next;
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

bool map_destroy(map_t *map)
{
    if(map == NULL)
        return false;

    map_t * tmp = NULL;

    while(map != NULL)
    {
        tmp = map;
        map = map->next;
        free(tmp->value);
        free(tmp->key);
        free(tmp);
    }

    return true;

}

char * 
map_get(map_t *map,char *key)
{
    if(map_len(map) < 1)
    {
        printf("Key not found.\n");
        return NULL;
    }

    map_t *tmp = map;

    while(tmp != NULL)
    {
        if(strcmp(key,tmp->key) == 0)
        {
            return tmp->value;
        }
        tmp = tmp->next;
    }

    printf("Key not found.\n");
    return NULL;
}