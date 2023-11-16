#include <stdio.h>

#include "urls.h"
#include "url_parser.h"
#include "map.h"
#include "mpesa.h"

#define BUFFLEN 4096



int main()
{
    //set_up_server("2000");

    //char * dn = get_path_from_url("http://localhost:8");

    //url_t *x = url_parser_parse("https://sandbox.localhost.co.ke:3000/?hello=world&niaje=poa");

    mpesa_do_stk_push("254716732614",1);
}