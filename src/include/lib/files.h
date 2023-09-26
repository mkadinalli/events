#pragma once

#include <mysql/mysql.h>
#include <json-c/json.h>


#include "../data-structures/ev_string.h"
#include "../data-structures/list.h"
#include "../data-structures/list.h"

char *
read_file_to_string(char *path);
MYSQL *
create_connection_from_a_file(MYSQL *sql_struct,
                              char *path_to_config);