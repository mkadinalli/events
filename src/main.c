#include <stdio.h>
#include <json-c/json.h>
#include <assert.h>
#include "./include/http/server.h"
#include <mysql/mysql.h>
#include "lib/files.h"
#include <stdbool.h>
#include "da/db.h"
#include "result_bind.h"



int main()
{
    set_up_server("2000");
}

/*

    result_bind *a = result_bind_create(1);
    result_bind_set_string(0, a, "my");

    execute_prepared_query("select cast(bin_to_uuid(id) as char) as id,username,name,email from users", NULL);

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
