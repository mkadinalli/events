#include "../../include/lib/ev_strings.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

char *string_create_copy(char *str)
{
    int len = strlen(str) + 1;
    char *res = malloc(sizeof(char) * len);
    strcpy(res, str);
    return res;
}

char *string_cover(char *str){
    size_t len = strlen(str);
    char *newstr = malloc(sizeof(char) * (len+3));

    newstr[0] = '\'';

    for(size_t i = 1; i < len+1;i++)
    {
        newstr[i] = str[i-1];
    }

    newstr[len+1] = '\'';
    newstr[len+2] = '\0';

    return newstr;
}


bool starts_with_word(char *word, char *str)
{
    bool truth = true;

    if (word == NULL || str == NULL)
        return false;

    if (strlen(word) > strlen(str))
        return false;

    for (size_t i = 0; i < strlen(word); i++)
    {
        if (word[i] != str[i])
            truth = false;
    }

    return truth;
}

char *string_removechar(char c, char *buffer, size_t len)
{
    string_t *temp = string_create();

    for (size_t i = 0; i < len; i++)
    {
        if (buffer[i] == c)
        {
            continue;
        }

        string_append(temp, buffer[i]);
    }
    string_append(temp, '\0');
    char *chars = temp->chars;
    free(temp);
    return chars;
}

char *string_replacechar(char previous, char current, char *buffer, size_t len)
{
    if(buffer == NULL) return NULL;
    
    string_t *temp = string_create();
    for (size_t i = 0; i < len; i++)
    {
        if (buffer[i] == previous)
        {
            string_append(temp, current);
     
            continue;
        }

        string_append(temp, buffer[i]);
    }
    string_append(temp, '\0');
    char *chars =string_create_copy(temp->chars);
    string_destroy(temp);
    return chars;
}

char *string_removechar_at(unsigned int pos, char *buffer, size_t len)
{
    string_t *temp = string_create();
    for (size_t i = 0; i < len; i++)
    {
        if (i == pos)
        {
            continue;
        }

        string_append(temp, buffer[i]);
    }
    string_append(temp, '\0');
    char *chars = temp->chars;
    free(temp);
    return chars;
}

int string_last_index_of(char c, char *str, unsigned int len)
{
    int pos = -1;
    int i;
    for (i = len - 1; i > -1; i--)
    {
        if (str[i] == c)
        {
            pos = i;
            break;
        };
    }
    return pos;
}

char *string_sub(unsigned int start, unsigned int stop, char *buffer, unsigned int len)
{
    string_t *temp = string_create();
    for (unsigned int i = 0; i < len; i++)
    {
        if (i >= start && i <= stop)
        {
            string_append(temp, buffer[i]);
        }

        continue;
    }
    string_append(temp, '\0');
    char *chars = temp->chars;
    free(temp);
    return chars;
}

bool endswith(char c, char *str)
{
    if (str[strlen(str) - 1] == c)
        return true;
    return false;
}

bool startswith(char c, char *str)
{
    if (str[0] == c)
        return true;
    // putchar('d');
    return false;
}

list_t *split(char delim, char *str, size_t str_size)
{
    list_t *myvec = list_create();
    string_t *bld = string_create();

    for (size_t i = 0; i < str_size; i++)
    {
        if (str[i] == delim)
        {
            string_append(bld, '\0');
            list_pushback(myvec, bld->chars);

            string_destroy(bld);
            bld = string_create();

            continue;
        }

        string_append(bld, str[i]);
    }
    string_append(bld, '\0');
    list_pushback(myvec, bld->chars);
    string_destroy(bld);
    return myvec;
}

list_t *split_lim(char delim, char *str, size_t str_size, int lim)
{
    list_t *myvec = list_create();
    string_t *bld = string_create();

    bool found = false;
    int times_found = 1;

    for (size_t i = 0; i < str_size; i++)
    {

        if ((str[i] == delim && !found) && times_found < lim)
        {
            string_append(bld, '\0');
            list_pushback(myvec, bld->chars);
            string_destroy(bld);
            bld = string_create();
            times_found++;
            if (times_found == lim)
                found = true;
            continue;
        }

        string_append(bld, str[i]);
    }

    string_append(bld, '\0');
    list_pushback(myvec, bld->chars);
    string_destroy(bld);
    return myvec;
}

char *setchar_at(char *str, char c, size_t pos, size_t len)
{
    if (pos >= len)
        return str;

    string_t *temp = string_create();
    for (size_t i = 0; i < len; i++)
    {
        if (i == pos)
        {
            string_append(temp, c);
            continue;
        }

        string_append(temp, str[i]);
    }
    string_append(temp, '\0');
    char *chars = temp->chars;
    free(temp);
    return chars;
}