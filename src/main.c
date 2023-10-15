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

#define BUFFLEN 4096



int main()
{
    if(mpesa_do_stk_push("254716732614",50000))
        puts("Success");
}