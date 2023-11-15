#include <stdio.h>

#include "urls.h"

#define BUFFLEN 4096



int main()
{
    //set_up_server("2000");

    char * dn = get_path_from_url("http://localhost:8");

    bool x = verify_url("https://localhost.com/");

    if(x)
        puts("valid url");
    else
        puts("inavlid url");

}