#pragma once

#include "http_client.h"
#include <json-c/json.h>

char *mpesa_get_access_token(char *consumer,char *secret);
bool mpesa_do_b2c(char *phone);