#include "../../include/lib/urls.h"
#include "../../include/data-structures/list.h"
#include "../../include/data-structures/map.h"
#include "../../include/data-structures/ev_string.h"


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

struct map_t *parse_url_query(char *query)
{
    list_t *param_parts = split('&',
                                    query,
                                    strlen(query));

    if (list_len(param_parts) == 0)
        return NULL;

    list_t *tmp = param_parts;

    map_t *params = map_create();

    while (tmp != NULL)
    {
        list_t *this_pair = split_lim('=', tmp->value, strlen(tmp->value),2);

        if (list_len(this_pair) != 2)
        {
            tmp = tmp->next;
            continue;
        }

        if(strlen(list_get(this_pair, 0)) <= 0 || strlen(list_get(this_pair, 1)) <= 0)
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

char *get_param_from_url(char *url,char *key)
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

    char * value = map_get(params,key);

    map_destroy(url_m);
    map_destroy(params);

    return value;
}
