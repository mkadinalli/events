#include <stdio.h>
#include "./include/string_buffer.h"
#include "./include/list.h"
#include "./include/map.h"

int 
main () 
{
    map_t *map = map_create();
    map_add(map,"vic","hello");
    map_add(map,"vic","hello");
    map_add(map,"vic","hello");
    map_print(map);

    printf("%d\n",map_len(map));

    map_destroy(map);

    return 0;
}
