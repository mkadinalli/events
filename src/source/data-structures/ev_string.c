#include "../../include/data-structures/ev_string.h"

#include <openssl/evp.h>

unsigned char *base64_encode(unsigned char *str,size_t len)
{
    int pl = 4*((len+2)/3);
    unsigned char *out = calloc(pl+1,1);
    int ol = EVP_EncodeBlock(out,str,len);
    if(pl != ol) return NULL;
    return out;
}


unsigned char *base64_decode(unsigned char *input,int length)
{
    int pl = 3*length/4;
    unsigned char *output = calloc(pl+1,1);
    int ol = EVP_DecodeBlock(output,input,length);
    if(pl != ol) return NULL;
    return output;
}


string_t *string_create()
{
    string_t *t = malloc(sizeof(string_t));
    t->chars = malloc(sizeof(char));
    t->chars[0] = '\0';
    return t;
}

string_t *string_create_from_string(char *string)
{
    int len = strlen(string);
    len++;
    string_t *t = malloc(sizeof(string_t));
    t->chars = malloc(sizeof(char) * len);
    strcpy(t->chars, string);
    t->chars[len] = '\0';

    return t;
}

bool string_append(string_t *str, char c)
{
    if (str != NULL)
    {

        int len = strlen(str->chars);

        str->chars = realloc(str->chars, len + 2);
        str->chars[len] = c;
        str->chars[len + 1] = '\0';

        return true;
    }

    return false;
}

bool string_destroy(string_t *str)
{
    if (str != NULL)
    {
        free(str->chars);
        free(str);
        return true;
    }
    return false;
}

bool string_pop(string_t *str)
{
    if (str == NULL)
        return false;
    str->chars[strlen(str->chars) - 1] = '\0';
    return true;
}
char string_back(string_t *str)
{
    return str->chars[strlen(str->chars) - 1];
}
char string_front(string_t *str)
{
    return str->chars[0];
}

void string_concat(string_t *dest, char *src, size_t len_src)
{
    for (size_t i = 0; i < len_src; i++)
    {
        string_append(dest, src[i]);
    }

    string_append(dest, '\0');
}