#pragma once

#include <mysql/mysql.h>
#include <json-c/json.h>
#include <stdio.h>
#include <threads.h>
#include "../lib/ev_strings.h"
#include "result_bind.h"

json_object * insert_into_payments(const char *user_id, const char *pub_id,char *conv_id,float amount);