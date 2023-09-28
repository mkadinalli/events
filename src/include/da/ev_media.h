#pragma once

#include <mysql/mysql.h>
#include <json-c/json.h>
#include <stdio.h>
#include <threads.h>
#include "../lib/ev_strings.h"
#include "result_bind.h"
#include "db.h"

json_object * insert_user_image_filename(char *user_id,char *file_name);