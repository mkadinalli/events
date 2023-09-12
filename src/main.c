#include <stdio.h>
#include "./include/string_buffer.h"
#include "./include/list.h"
#include "./include/map.h"
#include "./include/lib.h"
#include "./json.h"

int 
main () 
{
    char * json = "{'n a me': '', 'age'\n : 20, {'car':'buyer'}, \n'langs' : ['c':{'key':'value'} , 'c++' , 'java'],'obj':{'key':'value'}}";
    //char *json = "[{'hello':'world'},{'hello2':'world2'},{'hello3':'world3'}]";
    json_object_t * j = json_parse(json);

    //list_t *l = split(COMMA,json,strlen(json));

    map_print(j->j_pairs);
    //puts(json);
    //free(json);
    return 0;
}
