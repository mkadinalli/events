#pragma once

#include <json-c/json.h>


#include "../data-structures/ev_string.h"
#include "../data-structures/list.h"
#include "../data-structures/map.h"



json_object *
create_json_object_from_map(map_t *map);