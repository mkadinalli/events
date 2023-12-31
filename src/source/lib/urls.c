#include "../../include/lib/urls.h"
#include "../../include/data-structures/list.h"
#include "../../include/data-structures/map.h"
#include "../../include/data-structures/ev_string.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <regex.h>

struct map_t *parse_url_query(char *query)
{
    if(query == NULL) return NULL;

    if(strlen(query) < 2) return NULL;

    list_t *param_parts = split('&',
                                query,
                                strlen(query));

    if (list_len(param_parts) == 0)
        return NULL;

    list_t *tmp = param_parts;

    map_t *params = map_create();

    while (tmp != NULL)
    {
        list_t *this_pair = split_lim('=', tmp->value, strlen(tmp->value), 2);

        if (list_len(this_pair) != 2)
        {
            tmp = tmp->next;
            continue;
        }

        if (strlen(list_get(this_pair, 0)) <= 0 || strlen(list_get(this_pair, 1)) <= 0)
        {
            list_destroy(this_pair);

            tmp = tmp->next;
            continue;
        }

        map_add(params, list_get(this_pair, 0), list_get(this_pair, 1));

        list_destroy(this_pair);

        tmp = tmp->next;
    }

    list_destroy(param_parts);
    return params;
}

struct map_t *parse_url(char *url)
{
    list_t *url_parts = split_lim('?', url, strlen(url), 2);

    if (list_len(url_parts) != 2)
    {
        return NULL;
    }

    map_t *my_parts = map_create();
    map_add(my_parts, "url", list_get(url_parts, 0));
    map_add(my_parts, "query", list_get(url_parts, 1));

    list_destroy(url_parts);
    return my_parts;
}

char *get_param_part_from_url(char *url)
{
    list_t *url_parts = split_lim('?', url, strlen(url), 2);

    if (list_len(url_parts) != 2)
    {
        return NULL;
    }

    char *ret = list_get(url_parts, 1);

    list_destroy(url_parts);
    return ret;
}

char *get_param_from_url(char *url, char *key)
{
    map_t *url_m = parse_url(url);
    if (url_m == NULL)
    {
        return NULL;
    }

    if (map_len(url_m) != 2)
    {
        map_destroy(url_m);
        return NULL;
    }

    map_t *params = parse_url_query(map_get(url_m, "query"));

    if (params == NULL)
    {
        return NULL;
    }

    char *value = map_get(params, key);

    map_destroy(url_m);
    map_destroy(params);

    return value;
}

char *get_scheme_from_url(char *url)
{
    list_t *colon_splits = split(':', url, strlen(url));

    char *scheme = NULL;

    if (list_len(colon_splits) > 0)
        scheme = list_get(colon_splits, 0);

    list_destroy(colon_splits);

    return scheme;
}

char *get_domain_name_from_url(char *url)
{
    list_t *colon_splits = split(':', url, strlen(url));
    list_t *slash_splits = NULL;

    char *uri = NULL;

    if (list_len(colon_splits) > 1)
    {

        char *domain_double_slashed = list_get_ref(colon_splits, 1);
        int dds_len = strlen(domain_double_slashed);

        slash_splits = split('/', domain_double_slashed, dds_len);

        if (slash_splits)
        {

            uri = list_get(slash_splits, 2);
        }

        list_destroy(slash_splits);
    }

    list_destroy(colon_splits);

    return uri;
}

// http://localhost:2000/bla/bla
char *get_port_from_url(char *url)
{
    list_t *colon_splits = split_lim(':', url, strlen(url), 3);

    if (list_len(colon_splits) != 3)
    {
        list_destroy(colon_splits);
        return NULL;
    }

    char *port = NULL;

    char *last_part = list_get_ref(colon_splits, 2);

    list_t *slash_splits = split('/', last_part, strlen(last_part));

    if (list_len(slash_splits) > 0)
        port = list_get(slash_splits, 0);

    list_destroy(slash_splits);
    list_destroy(colon_splits);

    return port;
}


char *get_path_from_url(char *url)
{
    list_t *colon_splits = split_lim(':', url, strlen(url), 3);

    int c_split_len = list_len(colon_splits);

    char *path = NULL;

    char *last_part = list_get_ref(colon_splits, c_split_len - 1); //1

    list_t *slash_splits = NULL;

    if (c_split_len == 3)
    {
        slash_splits = split_lim('/', last_part, strlen(last_part), 2);
        path = list_get(slash_splits, 1);
    }
    else if (c_split_len == 2)
    {
        slash_splits = split_lim('/', last_part, strlen(last_part), 4);
        path = list_get(slash_splits, 3);
    }

    list_destroy(slash_splits);
    list_destroy(colon_splits);

    if(path == NULL) return path;

    if(strlen(path) == 0){
        free(path);
        path = NULL;

        string_t * t = string_create_from_string("/");
        path = string_create_copy(t->chars);
        string_destroy(t);
        return path;
    }

    char *pathtmp = path;

    path = string_add_char(pathtmp,'/',0);

    free(pathtmp);

    return path;
}


// http://localhost/bla/bla
bool verify_url(char *url)
{
    regex_t regex;
    int reti;
    bool out = false;

    reti = regcomp(&regex, "((http|https)://)[a-zA-Z0-9@:%._\\+~#?&//=]{2,256}\\.[a-z]{2,6}\\b([-a-zA-Z0-9@:%._\\+~#?&//=]*)", REG_EXTENDED);
    if (reti) {
        return false;
    }

    reti = regexec(&regex, url, 0, NULL, 0);
    if (!reti) {
        out = true;
    }

    regfree(&regex);

    return out;

}
