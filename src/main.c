#include <stdio.h>
#include "./include/string_buffer.h"
#include "./include/list.h"
#include "./include/map.h"
#include "./include/lib.h"

int 
main () 
{
    char * str = "hello world";
    char *a = string_sub(1,5,str,strlen(str));
    list_t *x = split_lim('l',str,strlen(str),2);

    list_print(x);
    list_destroy(x);
    //printf("%s\n",a);
    free(a);
    return 0;
}
