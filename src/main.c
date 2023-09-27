#include <stdio.h>
#include <json-c/json.h>
#include <assert.h>
#include "./include/http/server.h"
#include <mysql/mysql.h>
#include "lib/files.h"
#include <stdbool.h>
#include "da/db.h"



void eexecute_query(char *query,MYSQL *conn)
{
    MYSQL_STMT *stmt = mysql_stmt_init(conn);

    assert(stmt != NULL);

    //assert(!mysql_stmt_close(stmt));

    int status;

    status = mysql_stmt_prepare(stmt,query,strlen(query));

    assert(status == 0);

    MYSQL_RES *result_meta_data = mysql_stmt_result_metadata(stmt);

    assert(result_meta_data != NULL);

    unsigned int col_count = mysql_num_fields(result_meta_data);
    MYSQL_FIELD *columns = mysql_fetch_fields(result_meta_data);

    assert(columns != NULL);

    for(unsigned int i = 0; i < col_count;i++)
    {
        puts(columns[i].name);
    }


    MYSQL_BIND result_outputs[col_count];
    unsigned long lengths[col_count];
    bool is_null[col_count];
    bool error[col_count];

    for(unsigned int i = 0; i < col_count; i++)
    {
        result_outputs[i].buffer_type = columns[i].type;
        result_outputs[i].is_null = &(is_null[i]);
        result_outputs[i].length = &(lengths[i]);
        result_outputs[i].error = &(error[i]);
    }

    mysql_free_result(result_meta_data);
    assert(!mysql_stmt_close(stmt));
}





int main()
{
    conn_pool *cp = create_conn_pool(5);

    for(int i = 0; i < 100;i++)
    {

    MYSQL *c = cpool_get_connection(cp);

    assert(c != NULL);

    eexecute_query("select * from users",c);

        if(cpool_drop_connection(c,cp))
            puts("Connection dropped succesfully");
        else
            puts("Connection drop failed");
    }

}
