#include "../../include/data-structures/ev_string.h"

char *base64_encode(char *str)
{
    char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    char *res_string = malloc(1000 * sizeof(char));

    int index,no_of_bits = 0,padding = 0,val = 0,count = 0,temp;

    int i,j,k = 0;

    int len_str = strlen(str);

    for(i = 0; i < len_str; i+=3)
    {
        val = 0; count = 0; no_of_bits = 0;

        for(j = i; j < len_str && j <= i +2; j++)
        {
            val = val << 8;// 0000000

            val = val | str[j];

            count++;
        }

        no_of_bits = count * 8;

        padding = no_of_bits % 3;

        while(no_of_bits != 0)
        {
            //101 010 110 010 010 0
            if(no_of_bits >= 6)
            {
                //16
                temp = no_of_bits - 6;
                //10
                //1010 ->after shift
                index = (val >> temp) & 63;

                //111111
                //001010
                //------
                //001010

                no_of_bits -= 6;
            }
            else
            {
                temp = 6 - no_of_bits;

                index = (val << temp) & 63;

                //63 == 111111

                no_of_bits = 0;
            }

            res_string[k++] = charset[index];
        }
    }

    for(i = 1; i <= padding; i++)
    {
        res_string[k++] = '=';
    }

    res_string[k] = '\0';

    return res_string;
}


char *base64_decode(char *str)
{
    int len = strlen(str);
    int val = 0;

    int i;

    for(i = 0; i < len;i++)
    {
        val << 6;

        val = val | str[i] & 64;
    }

    int no_of_bits = i * 6;

    printf("bits = > %d\n",no_of_bits);
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