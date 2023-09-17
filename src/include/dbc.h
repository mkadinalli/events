#pragma once

#include <mysql/mysql.h>
#include <stdio.h>


void empty();

bool check_if_data_exists(MYSQL *conn,char *table_name,char *colname,char *value);

