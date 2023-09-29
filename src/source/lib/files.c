#include "../../include/lib/files.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

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


MYSQL *create_connection_from_a_file(char *path_to_config)
{
    MYSQL *sql_struct = mysql_init(NULL);

    char *str = read_file_to_string(path_to_config);
    struct json_object *jobj, *host, *password, *username, *db;

    // puts(str);

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
    sql_struct = mysql_real_connect(sql_struct,
                                    json_object_get_string(host),
                                    json_object_get_string(username),
                                    json_object_get_string(password),
                                    json_object_get_string(db),
                                    MYSQL_PORT,
                                    NULL,
                                    CLIENT_MULTI_STATEMENTS);

    return sql_struct;
}

bool open_file_to_append(char *path,char *value)
{
    FILE *ptr = NULL;
    if ((ptr = fopen(path, "a")))
    {
        fprintf(ptr,"%s\n",value);
        fclose(ptr);
        return true;
    }

    if ((ptr = fopen(path, "w")))
    {
        fprintf(ptr,"%s\n",value);
        fclose(ptr);
        return true;
    }

    return false;
}