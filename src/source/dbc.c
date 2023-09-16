#include "../include/dbc.h"

void empty()
{
    /*    MYSQL *conn = NULL;
    conn = mysql_init(conn);
    create_connection_from_a_file(&conn,
                                  "/home/vic/Desktop/ev2/events/config/config.json");

    if(conn == NULL)
    {
        puts("failed to connect");
        mysql_close(conn);
        exit(1);
    }

    char *query = "select * from users where id < 6";

    puts("conected");

    //mysql_set_server_option(conn,MYSQL_OPTION_MULTI_STATEMENTS_OFF);


    ///////////////////////////////////////////////////
    int status = mysql_query(conn,query);

    if(status)
    {
        puts("query failed");
        mysql_close(conn);
        exit(1);
    }

    MYSQL_RES *result = NULL;
    result = mysql_store_result(conn);

    if(result)
        puts("result found");
    else
        exit(1);


    //////////////////////////////////////////////////

    //MYSQL_BIND bind[1];
    MYSQL_STMT *stmt;

    stmt = mysql_stmt_init(conn);

    if(!stmt)
    {
        puts("out of memory");
        goto exit;
    }

    if(mysql_stmt_prepare(stmt,query,strlen(query)))
    {
        puts(mysql_stmt_error(stmt));
        goto exit;
    }

    //int l;

    //bind[0].buffer_type = MYSQL_TYPE_LONG;
    //bind[0].is_null = 0;
    //bind[0].length = 0;
    //bind[0].buffer = (char *)&l;

   //l = 103;

    if(mysql_stmt_bind_param(stmt,bind))
    {
        puts(mysql_stmt_error(stmt));
        goto exit;
    }

    if(mysql_stmt_execute(stmt))
    {
        puts("execute failed");
        goto exit;
    }

    MYSQL_BIND bind_results[2];
    MYSQL_RES *prepare_meta;

    prepare_meta = mysql_stmt_result_metadata(stmt);

    if(!prepare_meta)
    {
        puts("metadata not returned");
        exit(1);
    }

    int column_count;

    column_count = mysql_num_fields(prepare_meta);

    //printf("%d\n",column_count);

    memset(bind_results,0,sizeof bind_results);

    ////////
    long int_data;
    char str_data[50];
    bool is_null[2];
    bool error[2];
    unsigned long length[2];
    ///////

    bind_results[0].buffer = (char *)&int_data;
    bind_results[0].length = &length[0];
    bind_results[0].error = &error[0];
    bind_results[0].is_null = &is_null[0];
    bind_results[0].buffer_type = MYSQL_TYPE_LONG;

    bind_results[1].buffer = &str_data;
    bind_results[1].length = &length[1];
    bind_results[1].buffer_length = 50;
    bind_results[1].error = &error[1];
    bind_results[1].is_null = &is_null[1];
    bind_results[1].buffer_type = MYSQL_TYPE_STRING;

    if(mysql_stmt_bind_result(stmt,bind_results))
    {
        puts("bind failed");
        exit(1);
    }

    puts("fetching..");

    int row_count = 0;

    while(!mysql_stmt_fetch(stmt))
    {
        row_count++;
        if(!is_null[1]) printf("%s\n",str_data);
    }

    if(mysql_stmt_close(stmt))
    {
        //error
    }

    exit:
    mysql_close(conn);*/
}