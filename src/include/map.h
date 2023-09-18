#pragma once
#include <stdbool.h>
#include "./string_buffer.h"
#include "./lib.h"


typedef struct map_t map_t;
struct map_t{
    char * key;
    char * value;
    map_t * next;
};

map_t * map_create();
bool map_add(map_t *map,char *key,char *value);
bool map_destroy(map_t *map);
void fill_map_item(map_t * map,char * key,char *value);
void map_print(map_t *map);
int map_len(map_t * map);
char *map_get(map_t *map,char * key);