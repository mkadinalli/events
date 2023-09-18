#include "../include/lib.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

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
    char *chars = temp->chars;
    free(temp);
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
    string_t * bld = string_create();

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
    string_t * bld = string_create();

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
// char *terminate_string(char *str, size_t len){}

char *read_file_to_string(char *path)
{
    FILE *ptr = NULL;
    string_t *buf = string_create();

    if ((ptr = fopen(path, "r")))
    {
        char c;

        while ((c = fgetc(ptr)) != EOF)
        {
            string_append(buf, c);
        }

        fclose(ptr);
        char *btmp = buf->chars;
        free(buf);
        return btmp;
    }
    puts("file not found");
    free(buf);
    return NULL;
}

MYSQL *create_connection_from_a_file(MYSQL *sql_struct, char *path_to_config)
{
    char *str = read_file_to_string(path_to_config);
    struct json_object *jobj,*host, *password, *username, *db;

    //puts(str);

    jobj = json_tokener_parse(str);

    if (!(json_object_object_get_ex(jobj, "host", &host)))
    {
        fprintf(stderr, "Key not found : host\n");
        return NULL;
    }

    if (!(json_object_object_get_ex(jobj, "password", &password)))
    {
        fprintf(stderr, "Key not found : Password\n");
        return NULL;
    }

    if (!(json_object_object_get_ex(jobj, "database", &db)))
    {
        fprintf(stderr, "Key not found : database\n");
        return NULL;
    }

    if (!(json_object_object_get_ex(jobj, "username", &username)))
    {
        fprintf(stderr, "Key not found : username\n");
        return NULL;
    }

    free(str);
    sql_struct =  mysql_real_connect(sql_struct,
                               json_object_get_string(host),
                               json_object_get_string(username),
                               json_object_get_string(password),
                               json_object_get_string(db),
                               MYSQL_PORT,
                               NULL,
                               CLIENT_MULTI_STATEMENTS);

    return sql_struct;
}

bool starts_with_word(char *word,char *str)
{
    bool truth = true;

    if(word == NULL || str == NULL) return false;

    if(strlen(word) > strlen(str)) return false;

    for(size_t i = 0; i < strlen(word); i++)
    {
        if(word[i] != str[i]) truth = false;
    }

    return truth;
}


struct map_t * parse_url_query(char *query)
{
    list_t *param_parts = split_lim('&',
                                    query,
                                    strlen(query),
                                    2);
    
    if(list_len(param_parts) == 0)
        return NULL;
    
    list_print(param_parts);

    list_t * tmp = param_parts;

    map_t * params = map_create();

    while(tmp != NULL)
    {
        list_t * this_pair = split_lim('=',tmp->value,strlen(tmp->value),2);

        if(list_len(this_pair) != 2)
        {   tmp = tmp->next;
            continue;
        }
        
        map_add(params,list_get(this_pair,0),list_get(this_pair,1));

        list_destroy(this_pair);

        tmp = tmp->next;
    }

    list_destroy(param_parts);
    return params;
}

struct map_t * parse_url(char * url)
{
    list_t *url_parts = split_lim('?', url, strlen(url), 2);

    if(list_len(url_parts) != 2)
    {
        return NULL;
    }

    map_t *my_parts = map_create();
    map_add(my_parts,"url",list_get(url_parts,0));
    map_add(my_parts,"query",list_get(url_parts,1));

    list_destroy(url_parts);
    return my_parts;
}

json_object * create_json_object_from_map(map_t * map)
{
    if(map == NULL)
        return NULL;
        
    json_object * jobj = json_object_new_object();

    map_t *tmp = map;

    while(tmp != NULL)
    {
        json_object_object_add(jobj,tmp->key,json_object_new_string(tmp->value));
        tmp = tmp->next;
    }
    return jobj;
}

char * string_create_copy(char *str){
    int len = strlen(str)+1;
    char *res = malloc(sizeof(char) * len);
    strcpy(res,str);
    return res;
}
