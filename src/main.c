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

int main()
{
    map_t * m = map_create();
    map_add(m,"hello","world");
    map_add(m,"hi","there");

    create_json_object_from_map(m);

    // set_up_server("2000");
}
