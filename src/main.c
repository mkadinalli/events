#include <stdio.h>
#include "./include/string_buffer.h"
#include "./include/list.h"
#include "./include/map.h"

int 
main () 
{
    list_t* ls = list_create();
    list_pushback(ls,"value");
    list_destroy(ls);

    return 0;
}
