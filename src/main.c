#include <stdio.h>
#include "./include/string_buffer.h"
#include "./include/list.h"
#include "./include/map.h"
#include "./include/lib.h"
#include "./include/requests.h"
#include "./include/dbc.h"
#include <json-c/json.h>

int main()
{
    map_t *map = map_create();
    send_http_request(map,"localhost");
}
