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

    http_client_append_string("xcxcxcxcxcxcxcxcxcxcxcxcxcxppcxcxcxcxcxcxcxcxcxcxcxcxcxcxcxcxxcxcxcxcxcxcxcxcxcxccxcxcxcxcxcxcxcxcxcxcx",ct);
    http_client_set_method(GET,ct);
    http_client_set_address("localhost",ct);
    http_client_set_port("2000",ct);
    http_client_set_url("/",ct);

    http_client_set_header("User-Agent","PostmanRuntime/7.32.3",ct);
    http_client_set_header("Accept","*/*",ct);
    http_client_set_header("Postman-Token","cade1aa7-6db4-46cf-ba00-e1b63fa9ec2a",ct);
    http_client_set_header("Host","dummyjson.com:443",ct);
    //http_client_set_header("Accept-Encoding","gzip, deflate, br",ct);
    http_client_set_header("Cache-control","no-cache",ct);
    http_client_set_header("Connection","close",ct);
    http_client_set_header("Authorization","",ct);

    http_client_connect(ct);

    
    
}
