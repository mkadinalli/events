#include <stdio.h>
#include <json-c/json.h>
#include <assert.h>
#include "./include/http/server.h"
#include <mysql/mysql.h>
#include "lib/files.h"
#include <stdbool.h>
#include "da/db.h"
#include "result_bind.h"
#include <time.h>
#include "events-da.h"
#include "http_client.h"
#include <openssl/ssl.h>
#include "mpesa.h"
#include "gmail.h"
#include <openssl/pem.h>
#include <openssl/err.h>
#include "ev_time.h"
#include "urls.h"

#define BUFFLEN 4096



int main()
{
    //set_up_server("2000");

    char * dn = get_path_from_url("http://localhost:8/bla/bla");

    if(dn)
        puts(dn);
    else
        puts("inavlid url");

}