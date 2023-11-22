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
    map_t *m = map_create();
    map_add(m,"key","dGhlIHNhbXBsZSBub25jZQ==");

    char *result = createAcceptString(map_get(m,"key"));

    puts(result);
    set_up_server("2000");
}