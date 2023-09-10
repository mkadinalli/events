#pragma once

#include <stdbool.h>
#include <stdio.h>
#include "./string_buffer.h"

typedef struct list_t list_t;
struct list_t{
    char * value;
    list_t * next;
};

bool list_pushback(list_t * list,char * value);
bool list_pushfront(list_t **list,char *value);
list_t *list_create();
void list_print(list_t * list);
bool list_popback(list_t * list);
bool list_popfront(list_t ** list);
char * list_back(list_t * list);
char * list_front(list_t * list);
bool list_destroy(list_t * list);
int list_len(list_t *list);
void fill_list_item(list_t *list,char *str);