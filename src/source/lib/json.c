#include "../../include/lib/json.h"


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

json_object *create_json_object_from_map(map_t *map)
{
    if (map == NULL)
        return NULL;

    json_object *jobj = json_object_new_object();

    map_t *tmp = map;

    while (tmp != NULL)
    {
        json_object_object_add(jobj, tmp->key, json_object_new_string(tmp->value));
        tmp = tmp->next;
    }
    return jobj;
}