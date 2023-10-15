#pragma once

#include "http_client.h"
#include <json-c/json.h>

typedef struct lipa lipa;
struct lipa{
    unsigned char * pass_word;
    char * time;
};

char *mpesa_get_access_token(char *consumer,char *secret);
bool mpesa_do_stk_push(char * p_number,int amount);
lipa * mpesa_get_password();
 void *mpesa_destroy_password(lipa *l);