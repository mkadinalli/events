#pragma once

#include <mysql/mysql.h>
#include <stdio.h>
#include <threads.h>
#include "./lib.h"


void empty();

bool start_db_connection();
void drop_connection();
MYSQL ** get_connection();
bool check_if_data_exists(MYSQL *conn,char *table_name,char *colname,char *value);

