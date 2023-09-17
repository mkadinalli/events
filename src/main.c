#include <stdio.h>
#include "./include/string_buffer.h"
#include "./include/list.h"
#include "./include/map.h"
#include "./include/lib.h"
#include "./include/requests.h"
#include "./include/dbc.h"
#include "./include/server.h"
#include <json-c/json.h>
#include <time.h>

map_t * parse_url_query(char *query)
{
    list_t *param_parts = split_lim('&',
                                    query,
                                    strlen(query),
                                    2);
    
    if(list_len(param_parts) == 0)
        return NULL;
    

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

map_t * parse_url(char * url)
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

int main()
{

    // set_up_server("2000");
}
