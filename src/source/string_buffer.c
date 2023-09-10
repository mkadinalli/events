#include "../include/string_buffer.h"

string_t *string_create()
{
    string_t * t = malloc(sizeof(string_t));
    t->chars = '\0';
    return t;
}

bool string_append(string_t* str,char c)
{
    int len = strlen(str);
    printf("%d\n")
}

bool string_destroy(string_t *str)
{
    if(str != NULL)
        free(str);
        return true;
    
    return false;
}