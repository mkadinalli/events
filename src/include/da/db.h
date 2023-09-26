#pragma once

#include <mysql/mysql.h>
#include <json-c/json.h>
#include <stdio.h>
#include <threads.h>
#include "../lib/ev_strings.h"

/**
 * @brief check if username or password already exists in the DB
 * 
*/
bool check_if_user_data_exists(const char *username, const char *email);

/**
 * @brief Check if password is correct
 * 
 * @param identity - Email or Username
 * @param by_email - Set to true if identity is email, else false
*/
bool check_if_user_exists(char *identity, char *password, bool by_email);

/**
 * @brief Find number of rows returned from a query
 * @param query - SQL query
*/
int find_row_count(char *query);


/**
 * @brief execute a query
 * @param query - SQL query
*/
bool execute_query(char *query);


/**
 * @brief start db connection
 * 
*/
MYSQL *start_db_connection();