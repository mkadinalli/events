#pragma once

#include <mysql/mysql.h>
#include <json-c/json.h>
#include <stdio.h>
#include <threads.h>
#include "../lib/ev_strings.h"
#include <assert.h>

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
void execute_prepared_statement(MYSQL_STMT *stmt);


/**
 * @brief start db connection
 * 
*/
MYSQL *start_db_connection();


typedef struct db_conn db_conn;
struct db_conn{
    bool busy;
    MYSQL * connection;
    db_conn * next;
    unsigned long t_id;
    mtx_t dbmtx;
};

typedef struct conn_pool conn_pool;
struct conn_pool{
    db_conn * connections;
    cnd_t conn_condition;
    size_t busy_conns;
    mtx_t conn_mtx;
    size_t pool_size;
};

conn_pool * create_conn_pool(size_t size);
MYSQL *cpool_get_connection(conn_pool *cpool);
bool cpool_drop_connection(MYSQL *con ,conn_pool *cpool);
void cpool_destroy(conn_pool *cpool);

extern conn_pool *cpool;
