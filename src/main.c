#include <stdio.h>
#include "./include/string_buffer.h"
#include "./include/list.h"
#include "./include/map.h"

int 
main () 
{
    map_t *map = map_create();
    map_add(map,"vic","hello");
    map_add(map,"virc","hello");
    map_add(map,"vict","hello");
    map_add(map,"vici","hello");

    map_get(map,"vici");

    map_print(map);

    //printf("%d\n",map_len(map));

    list_t *ls = list_create();
    list_pushback(ls,"hey");
    list_pushback(ls,"barbiana");

    printf("%s\n",list_get(ls,1));


    map_destroy(map);

    return 0;
}
