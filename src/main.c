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
    struct json_object * jobj;

    char *str = "{'hello':'world}";

    printf("%s",str);

    jobj = json_tokener_parse(str);

    printf("%s\n",json_object_to_json_string_ext(jobj,JSON_C_TO_STRING_PRETTY));
}
