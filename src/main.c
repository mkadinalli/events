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

#define BUFFLEN 4096

int main()
{
    if(gmail_send_message("murimimlvictor@gmail.com","Niaje Mkuu bana"))
        puts("Message sent succesfully");

    return 0;
}