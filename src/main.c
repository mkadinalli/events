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

int main()
{
    http_client *ct = http_client_create();

    //http_client_append_string("xcxcxcxcxcxcxcxcxcxcxcxcxcxppcxcxcxcxcxcxcxcxcxcxcxcxcxcxcxcxxcxcxcxcxcxcxcxcxcxccxcxcxcxcxcxcxcxcxcxcx",ct);
    http_client_set_method(GET,ct);
    http_client_set_address("sandbox.safaricom.co.ke",ct);
    http_client_set_port("443",ct);
    http_client_set_url("/oauth/v1/generate?grant_type=client_credentials",ct);

    //http_client_set_header("User-Agent","PostmanRuntime/7.32.3",ct);
    //http_client_set_header("Postman-Token","cade1aa7-6db4-46cf-ba00-e1b63fa9ec2a",ct);
    http_client_set_header("Host","sandbox.safaricom.co.ke",ct);
    //http_client_set_header("Accept-Encoding","gzip, deflate, br",ct);
    http_client_set_header("Cache-control","no-cache",ct);
    http_client_set_header("Connection","close",ct);
    

    char *a =string_create_mpesa_auth("pqgen4fQJIx3bSYl17lNYgsBwkY8g44m","5U0icomXgD5mNgkm");
    //puts(base64_decode(a,strlen(a)));

    http_client_set_header("authorization",a,ct);



    http_client_connect(ct);

    //printf("%d\n",http_client_get_service_port("FTP"));


    
}
