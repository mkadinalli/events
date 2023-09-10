#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct string_t string_t;
struct string_t{
    char * chars;
};

string_t *string_create();
bool string_append(string_t* str,char c);
bool string_destroy(string_t * str);
