#include "../include/dbc.h"

void connect_to_db()
{
        const char *username = "vic";
    const char *password = "1234Victor";
    const char *host = "localhost";
    const char *database = "test";
    //const char *socket_name = "conn";
    //unsigned int sql_port = 2000;

    MYSQL *mysql = NULL;

    mysql = mysql_init(mysql);

    if ((mysql_real_connect(mysql,
                           host,
                           username,
                           password,
                           database,
                           MYSQL_PORT,
                           NULL,
                           CLIENT_MULTI_STATEMENTS)) == NULL)
    {
        puts("connection failed ");
        //printf("%s\n",mysql->);
        mysql_close(mysql);
        exit(EXIT_FAILURE);
    }

    int status = 0;

    status = mysql_query(mysql,"select * from users");

    if(status)
    {
        puts("could not execute statement");
        exit(EXIT_FAILURE);
    }

    MYSQL_RES * result;

    do {
        result = mysql_store_result(mysql);

        if(result)
        {
            mysql_free_result(result);
        }
        else
        {
            puts("could not get results");
            break;
        }

        status = mysql_next_result(mysql);

    } while (status == 0);

    mysql_close(mysql);
    
    return 0;
}