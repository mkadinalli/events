#pragma once
#include <stdbool.h>
typedef struct map_t map_t;
struct map_t{
    char * key;
    char * value;
    map_t * next;
};

map_t * map_create();
bool map_add(map_t *map,char *key,char *value);