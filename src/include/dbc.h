#pragma once

#include <mysql/mysql.h>
#include <stdio.h>
#include <threads.h>
#include "./lib.h"


void empty();

bool check_if_data_exists(MYSQL *conn,char *table_name,char *colname,char *value);
bool check_if_user_exists(MYSQL *conn,char *identity,char * password,bool by_email);
int find_row_count(MYSQL * conn,char *query);


