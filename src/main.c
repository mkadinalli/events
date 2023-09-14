#include <stdio.h>
#include "./include/string_buffer.h"
#include "./include/list.h"
#include "./include/map.h"
#include "./include/lib.h"
//#include "./json.h"
#include "./include/requests.h"
#include "./include/dbc.h"
#include <json-c/json.h>

int main()
{
    struct json_object * obj1,*obj2,*res;
    obj1 = json_object_new_object();
    obj2 = json_object_new_object();

    json_object_object_add(obj1,"one",json_object_new_int(123));
    json_object_object_add(obj2,"two",json_object_new_int(432));

    printf("%s\n",json_object_to_json_string(obj1));
    printf("%s\n",json_object_to_json_string(obj2));

    struct json_object * sub;

    json_object_object_get_ex(obj1,"one",&sub);
    

    res = json_object_new_array();

    json_object_array_add(res,obj1);
    json_object_array_add(res,obj2);

    printf("%s\n",json_object_to_json_string(res));

    json_object_put(obj1);
    json_object_put(obj2);
    json_object_put(res);

    
    
}
