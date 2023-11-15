#include <stdio.h>

#include "urls.h"
#include "url_parser.h"

#define BUFFLEN 4096



int main()
{
    //set_up_server("2000");

    //char * dn = get_path_from_url("http://localhost:8");

    url_t *x = url_parser_parse("https://localhost.com/");

    if(x->params == NULL)
        puts(x->domain);
    else
        puts("inavlid url");

    url_free(x);
        //puts("free successfull");

}