#pragma once

#include <stdbool.h>
#include <stdio.h>

typedef struct list_t list_t;
struct list_t{
    char * value;
    list_t * next;
};

bool list_pushback(char * value);
list_t list_create();
void list_print(list_t * list);
bool list_popback();
