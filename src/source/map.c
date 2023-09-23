#include "../include/map.h"
#include <stdio.h>

map_t *map_create()
{
    map_t *t = malloc(sizeof(map_t));
    t->key = NULL;
    t->value = NULL;
    t->next = NULL;

    return t;
}

bool map_add(map_t *map, char *key, char *value)
{
    if (map == NULL)
        return false;

    if (map_len(map) == 0)
    {
        fill_map_item(map, key, value);
        return true;
    }

    map_t *tmp = map;
    map_t *new_map = map_create();
    while (tmp->next != NULL)
    {
        if (strcmp(tmp->key, key) == 0)
        {
            printf("Key exists.\n");
            new_map = NULL;
            return false;
        }

        tmp = tmp->next;
    }

    if (strcmp(tmp->key, key) == 0)
    {
        printf("Key exists.\n");
        new_map = NULL;
        return false;
    }

    fill_map_item(new_map, key, value);

    tmp->next = new_map;
    return true;
}

int map_len(map_t *list)
{
    if (list == NULL)
        return -1;

    int size = 0;
    map_t *temp_list = list;
    while (temp_list->next != NULL)
    {
        temp_list = temp_list->next;
        size++;
    }

    if (temp_list->value != NULL)
        size++;

    return size;
}

void map_print(map_t *map)
{
    if (map == NULL)
        return;

    map_t *tmp = map;
    while (tmp != NULL)
    {
        printf("%s : %s\n", tmp->key, tmp->value);
        tmp = tmp->next;
    }
}

void fill_map_item(map_t *map, char *key, char *value)
{
    map->key = string_create_copy(key);
    map->value = string_create_copy(value);
    map->next = NULL;
}

bool map_destroy(map_t *map)
{
    if (map == NULL)
        return false;

    map_t *tmp = NULL;

    while (map != NULL)
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
map_get(map_t *map, char *key)
{
    if (map_len(map) < 1)
    {
        return NULL;
    }

    map_t *tmp = map;

    while (tmp != NULL)
    {
        if (strcmp(key, tmp->key) == 0)
        {
            return string_create_copy(tmp->value);
        }
        tmp = tmp->next;
    }
    return NULL;
}