#include <stdio.h>
#include "./include/string_buffer.h"
#include "./include/list.h"
#include "./include/map.h"
#include "./include/lib.h"
#include "./include/requests.h"
#include "./include/dbc.h"
#include "./include/server.h"
#include <json-c/json.h>
#include <time.h>

int main()
{
    http_res * h = malloc(sizeof(http_res));
    h->code = 200;
    h->code_name = "OK";
    h->content_length = 500;
    h->content_type = "application/json";
    h->http_version = "1.1";

    char * res = write_http_header_from_struct(h);

    puts(res);
    //set_up_server("3000");

}
