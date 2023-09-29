#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


#include "../lib/ev_strings.h"


typedef struct string_t string_t;
struct string_t
{
    char *chars;
};

string_t *string_create();
string_t *string_create_from_string(char *string);
bool string_append(string_t *str, char c);
bool string_destroy(string_t *str);
bool string_pop(string_t *str);
char string_back(string_t *str);
char string_front(string_t *str);
void string_concat(string_t *dest, char *src, size_t len);
