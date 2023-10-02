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

    http_client_append_string("{\"hello\" : \"world\"}",ct);

    puts(ct->body);
}
