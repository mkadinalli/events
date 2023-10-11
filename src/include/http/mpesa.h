#pragma once

#include "http_client.h"
#include <json-c/json.h>

char *mpesa_get_acces_token(char *consumer,char *secret);