#pragma once

#include <stdio.h>

#include "./list.h"
#include "./map.h"
#include "./string.h"
#include "./lib.h"

enum SYMBOLS{
    CURLY_OPEN_BRACKETS = '{',
    CURLY_CLOSE_BRACKETS = '}',
    SQUARE_OPEN_BRACKETS = '[',
    SQUARE_CLOSE_BRACKETS = ']',
    COLON = ':',
    COMMA = ',',
    SPECIAL = '|',
    SINGLE_QUOTE = '\'',
    DOUBLE_QUOTE = '"',
    SPACE = ' ',
    TAB = '\t',
    LINE_BREAK = '\n'
};


typedef struct json_array_t json_array_t;
struct json_array_t{
    list_t * j_objects;
};

typedef struct json_object_t json_object_t;
struct json_object_t{
    map_t * j_pairs;
};


char* json_check_syntax(char * j_string);
json_array_t *json_array(char * j_string);
json_object_t *json_object(char *j_string);
void *json_parse(char *j_string);
