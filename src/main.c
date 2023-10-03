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

int main()
{
    http_client *ct = http_client_create();

    //http_client_append_string("{\"hello\" : \"world\"}",ct);
    http_client_set_method(GET,ct);
    http_client_set_address("www.google.com",ct);
    http_client_set_port("443",ct);
    http_client_set_url("/search?channel=fs&client=ubuntu-sn&q=how+to+check+coredump+in+ubuntu",ct);

    http_client_set_header("post","localhost",ct);
    http_client_set_header("Connection","close",ct);

    http_client_connect(ct);

}
