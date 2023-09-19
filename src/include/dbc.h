#pragma once

#include <mysql/mysql.h>
#include <stdio.h>
#include <threads.h>
#include "./lib.h"

void empty();

bool check_if_user_data_exists(const char *username, const char *email);
bool check_if_user_exists(char *identity, char *password, bool by_email);
int find_row_count(char *query);
bool inser_into_users(const char *name, const char *username, const char *email, const char *password);
bool execute_query(char *query);
MYSQL *start_db_connection();
