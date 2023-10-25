#pragma once

#include <mysql/mysql.h>
#include <json-c/json.h>
#include <stdio.h>
#include <threads.h>
#include "../lib/ev_strings.h"
#include "db.h"
#include "result_bind.h"

json_object * insert_into_payments(const char *user_id, const char *pub_id,char *m_id,char *c_id,int amount);

json_object *verify_payment(const char *m_id,const char *c_id,const char *t_id);