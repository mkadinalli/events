#include <stdio.h>
#include "./include/string_buffer.h"
#include "./include/list.h"
#include "./include/map.h"
#include "./include/lib.h"
#include "./include/requests.h"
#include "./include/dbc.h"
#include "./include/server.h"
#include <json-c/json.h>
#include <time.h>

int main()
{
    MYSQL *conn = NULL;
    conn = mysql_init(conn);
    create_connection_from_a_file(&conn,
                                  "/home/vic/Desktop/ev2/events/config/config.json");

    if(conn == NULL)
    {
        puts("failed to connect");
        mysql_close(conn);
        exit(1);
    }

    char * query = read_file_to_string("/home/vic/Desktop/ev2/events/schema/scema.sql");

    if(!mysql_real_query(conn,query,strlen(query)))
    {
        puts(mysql_error(conn));
    }

    mysql_close(conn);
}
