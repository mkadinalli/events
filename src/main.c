#include <stdio.h>

#include "urls.h"
#include "url_parser.h"
#include "map.h"
#include "mpesa.h"
#include "web_sock.h"
#include "res_builder.h"

#define BUFFLEN 4096



int main()
{
    response_builder *r = response_builder_create();
    response_builder_set_code(r,"200");
    response_builder_set_status_name(r,"OK");
    response_builder_set_body(r,"hello world");

    char *c = response_builder_to_string(r);

    if(c) puts(c);
    //set_up_server("2000");
}