#include <stdio.h>
#include "./include/string_buffer.h"
#include "./include/list.h"
#include "./include/map.h"
#include "./include/lib.h"
#include "./json.h"

int 
main () 
{
    char * json = "{'name' 0:'vic' , 'age'\n : 20, {'car':'buyer'}, \n'langs' : ['c':{'key':'value'} , 'c++' , 'java'],'obj':{'key':'value'}}";
    
    //char * json = "{'dj':'vic','laove':'baby'}";
    json_check_syntax(json);

    return 0;
}
