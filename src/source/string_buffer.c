#include "../include/string_buffer.h"

string_t *string_create()
{
    string_t * t = malloc(sizeof(string_t));
    t->chars = malloc(sizeof(char));
    t->chars[0] = '\0';
    return t;
}

string_t *string_create_from_string(char * string)
{
    int len = strlen(string);
    len++;
    string_t *t = malloc(sizeof(string_t));
    t->chars = malloc(sizeof(char) * len);
    strcpy(t->chars,string);
    t->chars[len] = '\0';

    return t;

    printf("%d\n",len);
}

bool string_append(string_t* str,char c)
{
    if(str != NULL){
        int len = strlen(str->chars);
        str = realloc(str,len+1);
        str->chars[len] = c;
        str->chars[len+1] = '\0';

        return true;
    }

    return false;
}

bool string_destroy(string_t *str)
{
    if(str != NULL){
        free(str->chars);
        free(str);
        return true;
    }
    return false;
}