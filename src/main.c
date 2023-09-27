#include <stdio.h>
#include <json-c/json.h>
#include <assert.h>
#include "./include/http/server.h"
#include <mysql/mysql.h>
#include "lib/files.h"
#include <stdbool.h>
#include "da/db.h"
#include "result_bind.h"

MYSQL_STMT *execute_prepared_query(char *query, result_bind *params)
{
    assert(query != NULL);

    conn_pool *cpool = create_conn_pool(2);
    MYSQL *conn = cpool_get_connection(cpool);

    MYSQL_STMT *stmt = NULL;
    stmt = mysql_stmt_init(conn);

    assert(stmt != NULL);

    mysql_stmt_prepare(stmt, query, strlen(query));

    // assert(result_bind_get_size(params) > 0);

    if (params != NULL)
    {
        int param_count = mysql_stmt_param_count(stmt);
        assert(param_count == result_bind_get_size(params));

        MYSQL_BIND p_bind[param_count];

        for (int i = 0; i < param_count; i++)
        {
            unsigned long len = strlen(result_bind_get_at(i, params)->value);

            p_bind[i].buffer = result_bind_get_at(i, params)->value;
            p_bind[i].buffer_type = result_bind_get_at(i, params)->type_name;
            p_bind[i].is_null = 0;
            p_bind[i].length = &len;
            p_bind[i].buffer_length = 100;
        }

        bool status = mysql_stmt_bind_param(stmt, p_bind);

        assert(!status);
    }

    execute_prepared_statement(stmt);

    return NULL;
}

int main()
{
    result_bind *a = result_bind_create(1);
    result_bind_set_string(0, a, "my");

    execute_prepared_query("select cast(bin_to_uuid(id) as char) as id from users", NULL);
}

/*

    conn_pool *cp = create_conn_pool(5);

    for(int i = 0; i < 10;i++)
    {

    MYSQL *c = cpool_get_connection(cp);

    assert(c != NULL);

    eexecute_query("select cast(bin_to_uuid(id) as char) as id ,name,username,email from users",c);

        if(cpool_drop_connection(c,cp))
            puts("Connection dropped succesfully");
        else
            puts("Connection drop failed");
    }

*/
