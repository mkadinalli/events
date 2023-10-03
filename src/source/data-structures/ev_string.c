#include "../../include/data-structures/ev_string.h"

char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

char *base64_encode(const char *str,size_t len)
{

    char *out,*pos;
    const char *end,*in;

    size_t olen;
    int line_len;

    olen = len * 4 / 3 + 4;

    olen += 72;
    olen++;

    if(olen < len)
        return NULL;
    
    out = malloc(olen);

    if(out == NULL)
        return NULL;

    end = str+len;
    in = str;
    pos = out;

    line_len = 0;

    while(end - in >= 3)
    {
        *pos++ = charset[in[0] >> 2];
        *pos++ = charset[((in[0] & 0x03) << 4) | (in[1] >> 4)];
        *pos++ = charset[((in[1] & 0x0f) << 2) | (in[2] >> 6)];
        *pos++ = charset[in[2] & 0x3f];
        in += 3;

        line_len += 4;

        if(line_len >= 72)
        {
            *pos++ = '\n';
            line_len = 0;
        }   
    }

    if(end - in)
    {
        *pos++ = charset[in[0] >> 2];

        if(end - in == 1)
        {
            *pos++ = charset[(in[0] & 0x03) << 4];
            *pos++ = '=';
        }else{
            *pos++ = charset[((in[0] & 0x03) << 4) | (in[1] >> 4)];
            *pos++ = charset[(in[1] & 0x0f) << 2];
        }

        *pos++ = '=';
        line_len += 4;
    }

    *pos = '\0';

    return out;
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

    printf("%d\n", len);
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