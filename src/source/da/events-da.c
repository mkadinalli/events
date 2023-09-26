#include "../../include/da/events-da.h"
#include "../../include/lib/files.h"

bool insert_into_published(const char *title,
                          const char *description,
                          const char *venue,
                          const char *event_date,
                          const char *deadline_date,
                          const char *publisher_id)
{
    MYSQL *conn = NULL;
    conn = mysql_init(conn);
    conn = create_connection_from_a_file(conn,
                                         "/home/vic/Desktop/ev2/events/config/config.json");

    if (conn == NULL)
    {
        puts("failed to connect to db");
        return false;
    }

    char *query = "insert into published (title,description,venue,event_date,deadline_date,publisher_id) values (?,?,?,?,?,uuid_to_bin(?))";

    unsigned long title_l = strlen(title),
                  description_l = strlen(description),
                  venue_l = strlen(venue),
                  ev_d_l = strlen(event_date),
                  de_d_l = strlen(deadline_date),
                  p_id_l = strlen(publisher_id);

    MYSQL_BIND bind[6];
    MYSQL_STMT *stmt;

    stmt = mysql_stmt_init(conn);

    if (!stmt)
    {
        mysql_close(conn);
        return false;
    }

    if (mysql_stmt_prepare(stmt, query, strlen(query)))
    {
        goto exit_with_error;
    }

    bind[0].buffer_type = MYSQL_TYPE_STRING;
    bind[0].is_null = 0;
    bind[0].length = &title_l;
    bind[0].buffer_length = 100;
    bind[0].buffer = title;

    bind[1].buffer_type = MYSQL_TYPE_STRING;
    bind[1].is_null = 0;
    bind[1].length = &description_l;
    bind[1].buffer_length = 1000;
    bind[1].buffer = description;

    bind[2].buffer_type = MYSQL_TYPE_STRING;
    bind[2].is_null = 0;
    bind[2].length = &venue_l;
    bind[2].buffer_length = 100;
    bind[2].buffer = venue;

    bind[3].buffer_type = MYSQL_TYPE_STRING;
    bind[3].is_null = 0;
    bind[3].length = &ev_d_l;
    bind[3].buffer_length = 100;
    bind[3].buffer = event_date;

    bind[4].buffer_type = MYSQL_TYPE_STRING;
    bind[4].is_null = 0;
    bind[4].length = &de_d_l;
    bind[4].buffer_length = 100;
    bind[4].buffer = deadline_date;

    bind[5].buffer_type = MYSQL_TYPE_STRING;
    bind[5].is_null = 0;
    bind[5].length = &p_id_l;
    bind[5].buffer_length = 100;
    bind[5].buffer = publisher_id;

    if (mysql_stmt_bind_param(stmt, bind))
    {
        puts("An error occured");
        goto exit_with_error;
    }

    if (mysql_stmt_execute(stmt))
    {
        goto exit_with_error;
    }

    puts(mysql_stmt_error(stmt));
    mysql_stmt_close(stmt);
    return true;

exit_with_error:
    puts(mysql_stmt_error(stmt));
    mysql_stmt_close(stmt);
    return false;
}


json_object *select_from_published(const char *user_id,
                                   const char *last_time,
                                   const char *last_query_time)
{
    MYSQL *conn = NULL;
    conn = mysql_init(conn);
    conn = create_connection_from_a_file(conn,
                                         "/home/vic/Desktop/ev2/events/config/config.json");

    if (conn == NULL)
    {
        puts("failed to connect to db");
        goto exit;
    }

    char *query = "call get_many_published(? ,? ,?)";

    unsigned long user_l = strlen(user_id),
                  lt_l = strlen(last_time),
                  ltq_l = strlen(last_query_time);

    MYSQL_BIND bind[3];
    MYSQL_STMT *stmt;

    stmt = mysql_stmt_init(conn);

    if (!stmt)
    {
        puts("out of memory");
        goto exit;
    }

    if (mysql_stmt_prepare(stmt, query, strlen(query)))
    {
        puts(mysql_stmt_error(stmt));
        goto exit;
    }

    bind[0].buffer_type = MYSQL_TYPE_STRING;
    bind[0].is_null = 0;
    bind[0].length = &user_l;
    bind[0].buffer = user_id;
    bind[0].buffer_length = 100;

    bind[1].buffer_type = MYSQL_TYPE_STRING;
    bind[1].is_null = 0;
    bind[1].length = &lt_l;
    bind[1].buffer = last_time;
    bind[1].buffer_length = 100;

    bind[2].buffer_type = MYSQL_TYPE_STRING;
    bind[2].is_null = 0;
    bind[2].length = &ltq_l;
    bind[2].buffer = last_query_time;
    bind[2].buffer_length = 100;

    // l = 103;

    if (mysql_stmt_bind_param(stmt, bind))
    {
        puts(mysql_stmt_error(stmt));
        goto exit;
    }

    if (mysql_stmt_execute(stmt))
    {
        puts(mysql_stmt_error(stmt));
        goto exit;
    }

    // printf("%d\n",column_count);


    MYSQL_BIND bind_results[11];
    memset(bind_results, 0, sizeof bind_results);

    ////////
    char id[100],
        title[100],
        description[10000],
        venue[100],
        publisher_id[100],
        date_created[100],
        event_date[100],
        deadline_date[100],
        time_queried[100];

    long subscriptions, stars;

    bool is_null[11];
    bool error[11];
    unsigned long length[11];
    ///////

    bind_results[9].buffer = &subscriptions;
    bind_results[9].length = &length[9];
    bind_results[9].error = &error[9];
    bind_results[9].is_null = &is_null[9];
    bind_results[9].buffer_type = MYSQL_TYPE_LONG;

    bind_results[10].buffer = &stars;
    bind_results[10].length = &length[10];
    bind_results[10].error = &error[10];
    bind_results[10].is_null = &is_null[10];
    bind_results[10].buffer_type = MYSQL_TYPE_LONG;

    bind_results[0].buffer = id;
    bind_results[1].buffer = title;
    bind_results[2].buffer = description;
    bind_results[3].buffer = venue;
    bind_results[4].buffer = publisher_id;
    bind_results[5].buffer = date_created;
    bind_results[6].buffer = event_date;
    bind_results[7].buffer = deadline_date;
    bind_results[8].buffer = time_queried;

    for (int i = 0; i < 9; i++)
    {
        bind_results[i].length = &length[i];
        bind_results[i].buffer_length = i == 2 ? 1000 : 100;
        bind_results[i].error = &error[i];
        bind_results[i].is_null = &is_null[i];
        bind_results[i].buffer_type = MYSQL_TYPE_STRING;
    }

    if (mysql_stmt_bind_result(stmt, bind_results))
    {
        puts("bind failed");
        goto exit;
    }

    json_object * res = json_object_new_array();

    while (!mysql_stmt_fetch(stmt))
    {
        json_object * this_row = json_object_new_object();

        json_object_object_add(this_row,"id",json_object_new_string(id));
        json_object_object_add(this_row,"title",json_object_new_string(title));
        json_object_object_add(this_row,"description",json_object_new_string(description));
        json_object_object_add(this_row,"venue",json_object_new_string(venue));
        json_object_object_add(this_row,"event_date",json_object_new_string(event_date));
        json_object_object_add(this_row,"deadline",json_object_new_string(deadline_date));
        json_object_object_add(this_row,"publisher_id",json_object_new_string(publisher_id));
        json_object_object_add(this_row,"stars",json_object_new_uint64(stars));
        json_object_object_add(this_row,"subscriptions",json_object_new_uint64(subscriptions));
        json_object_object_add(this_row,"time_queried",json_object_new_string(time_queried));

        json_object_array_add(res,json_object_get(this_row));

        json_object_put(this_row);
    }

    mysql_stmt_close(stmt);
    mysql_close(conn);
    return res;

exit:
    mysql_stmt_close(stmt);
    mysql_close(conn);
    return NULL;

}


json_object *select_one_from_published(const char *id_)
{
    MYSQL *conn = NULL;
    conn = mysql_init(conn);
    conn = create_connection_from_a_file(conn,
                                         "/home/vic/Desktop/ev2/events/config/config.json");

    if (conn == NULL)
    {
        puts("failed to connect to db");
        goto exit;
    }

    char *query = "call get_one_published(?)";

    unsigned long id_l = strlen(id_);

    MYSQL_BIND bind[1];
    MYSQL_STMT *stmt;

    stmt = mysql_stmt_init(conn);

    if (!stmt)
    {
        puts("out of memory");
        goto exit;
    }

    if (mysql_stmt_prepare(stmt, query, strlen(query)))
    {
        puts(mysql_stmt_error(stmt));
        goto exit;
    }

    bind[0].buffer_type = MYSQL_TYPE_STRING;
    bind[0].is_null = 0;
    bind[0].length = &id_l;
    bind[0].buffer = id_;
    bind[0].buffer_length = 100;


    if (mysql_stmt_bind_param(stmt, bind))
    {
        puts(mysql_stmt_error(stmt));
        goto exit;
    }

    if (mysql_stmt_execute(stmt))
    {
        puts(mysql_stmt_error(stmt));
        goto exit;
    }

    // printf("%d\n",column_count);

    MYSQL_BIND bind_results[11];
    memset(bind_results, 0, sizeof bind_results);

    ////////
    char id[100],
        title[100],
        description[10000],
        venue[100],
        publisher_id[100],
        date_created[100],
        event_date[100],
        deadline_date[100],
        time_queried[100];

    long subscriptions, stars;

    bool is_null[11];
    bool error[11];
    unsigned long length[11];
    ///////

    bind_results[9].buffer = &subscriptions;
    bind_results[9].length = &length[9];
    bind_results[9].error = &error[9];
    bind_results[9].is_null = &is_null[9];
    bind_results[9].buffer_type = MYSQL_TYPE_LONG;

    bind_results[10].buffer = &stars;
    bind_results[10].length = &length[10];
    bind_results[10].error = &error[10];
    bind_results[10].is_null = &is_null[10];
    bind_results[10].buffer_type = MYSQL_TYPE_LONG;

    bind_results[0].buffer = id;
    bind_results[1].buffer = title;
    bind_results[2].buffer = description;
    bind_results[3].buffer = venue;
    bind_results[4].buffer = publisher_id;
    bind_results[5].buffer = date_created;
    bind_results[6].buffer = event_date;
    bind_results[7].buffer = deadline_date;
    bind_results[8].buffer = time_queried;

    for (int i = 0; i < 9; i++)
    {
        bind_results[i].length = &length[i];
        bind_results[i].buffer_length = i == 2 ? 1000 : 100;
        bind_results[i].error = &error[i];
        bind_results[i].is_null = &is_null[i];
        bind_results[i].buffer_type = MYSQL_TYPE_STRING;
    }

    if (mysql_stmt_bind_result(stmt, bind_results))
    {
        puts("bind failed");
        goto exit;
    }

    json_object * res = json_object_new_array();

    while (!mysql_stmt_fetch(stmt))
    {
        json_object * this_row = json_object_new_object();

        json_object_object_add(this_row,"id",json_object_new_string(id));
        json_object_object_add(this_row,"title",json_object_new_string(title));
        json_object_object_add(this_row,"description",json_object_new_string(description));
        json_object_object_add(this_row,"venue",json_object_new_string(venue));
        json_object_object_add(this_row,"event_date",json_object_new_string(event_date));
        json_object_object_add(this_row,"deadline",json_object_new_string(deadline_date));
        json_object_object_add(this_row,"publisher_id",json_object_new_string(publisher_id));
        json_object_object_add(this_row,"stars",json_object_new_uint64(stars));
        json_object_object_add(this_row,"subscriptions",json_object_new_uint64(subscriptions));
        json_object_object_add(this_row,"time_queried",json_object_new_string(time_queried));

        json_object_array_add(res,json_object_get(this_row));

        json_object_put(this_row);
    }

    mysql_stmt_close(stmt);
    mysql_close(conn);
    return res;

exit:
    mysql_stmt_close(stmt);
    mysql_close(conn);
    return NULL;
}


bool update_published(
                            const char *title,
                          const char *description,
                          const char *venue,
                          const char *event_date,
                          const char *deadline_date,
                          const char *publisher_id,     const char *id)
{
    MYSQL *conn = NULL;
    conn = mysql_init(conn);
    conn = create_connection_from_a_file(conn,
                                         "/home/vic/Desktop/ev2/events/config/config.json");

    if (conn == NULL)
    {
        puts("failed to connect to db");
        return false;
    }

    char *query = "update published set title = ?,description = ?,venue = ? ,event_date = ?,deadline_date = ?,publisher_id = uuid_to_bin(?) where id = uuid_to_bin(?)";

    unsigned long title_l = strlen(title),
                  description_l = strlen(description),
                  venue_l = strlen(venue),
                  ev_d_l = strlen(event_date),
                  de_d_l = strlen(deadline_date),
                  p_id_l = strlen(publisher_id),
                  id_l = strlen(id);

    MYSQL_BIND bind[7];
    MYSQL_STMT *stmt;

    stmt = mysql_stmt_init(conn);

    if (!stmt)
    {
        mysql_close(conn);
        return false;
    }

    if (mysql_stmt_prepare(stmt, query, strlen(query)))
    {
        goto exit_with_error;
    }

    bind[0].buffer_type = MYSQL_TYPE_STRING;
    bind[0].is_null = 0;
    bind[0].length = &title_l;
    bind[0].buffer_length = 100;
    bind[0].buffer = title;

    bind[1].buffer_type = MYSQL_TYPE_STRING;
    bind[1].is_null = 0;
    bind[1].length = &description_l;
    bind[1].buffer_length = 1000;
    bind[1].buffer = description;

    bind[2].buffer_type = MYSQL_TYPE_STRING;
    bind[2].is_null = 0;
    bind[2].length = &venue_l;
    bind[2].buffer_length = 100;
    bind[2].buffer = venue;

    bind[3].buffer_type = MYSQL_TYPE_STRING;
    bind[3].is_null = 0;
    bind[3].length = &ev_d_l;
    bind[3].buffer_length = 100;
    bind[3].buffer = event_date;

    bind[4].buffer_type = MYSQL_TYPE_STRING;
    bind[4].is_null = 0;
    bind[4].length = &de_d_l;
    bind[4].buffer_length = 100;
    bind[4].buffer = deadline_date;

    bind[5].buffer_type = MYSQL_TYPE_STRING;
    bind[5].is_null = 0;
    bind[5].length = &p_id_l;
    bind[5].buffer_length = 100;
    bind[5].buffer = publisher_id;


    bind[6].buffer_type = MYSQL_TYPE_STRING;
    bind[6].is_null = 0;
    bind[6].length = &id_l;
    bind[6].buffer_length = 100;
    bind[6].buffer = id;

    if (mysql_stmt_bind_param(stmt, bind))
    {
        puts("An error occured");
        goto exit_with_error;
    }

    if (mysql_stmt_execute(stmt))
    {
        goto exit_with_error;
    }

    puts(mysql_stmt_error(stmt));
    mysql_stmt_close(stmt);
    return true;

exit_with_error:
    puts(mysql_stmt_error(stmt));
    mysql_stmt_close(stmt);
    return false;
}


json_object *get_published_by_user_id(char *id_,char *last_time)
{
    MYSQL *conn = NULL;
    conn = mysql_init(conn);
    conn = create_connection_from_a_file(conn,
                                         "/home/vic/Desktop/ev2/events/config/config.json");

    if (conn == NULL)
    {
        puts("failed to connect to db");
        goto exit;
    }

    char *query = "call get_pub_for_user(?,?)";

    unsigned long id_l = strlen(id_),time_l = strlen(last_time);

    MYSQL_BIND bind[2];
    MYSQL_STMT *stmt;

    stmt = mysql_stmt_init(conn);

    if (!stmt)
    {
        puts("out of memory");
        goto exit;
    }

    if (mysql_stmt_prepare(stmt, query, strlen(query)))
    {
        puts(mysql_stmt_error(stmt));
        goto exit;
    }

    bind[0].buffer_type = MYSQL_TYPE_STRING;
    bind[0].is_null = 0;
    bind[0].length = &id_l;
    bind[0].buffer = id_;
    bind[0].buffer_length = 100;

    bind[1].buffer_type = MYSQL_TYPE_STRING;
    bind[1].is_null = 0;
    bind[1].length = &time_l;
    bind[1].buffer = last_time;
    bind[1].buffer_length = 100;


    if (mysql_stmt_bind_param(stmt, bind))
    {
        puts(mysql_stmt_error(stmt));
        goto exit;
    }

    if (mysql_stmt_execute(stmt))
    {
        puts(mysql_stmt_error(stmt));
        goto exit;
    }

    // printf("%d\n",column_count);

    MYSQL_BIND bind_results[6];
    memset(bind_results, 0, sizeof bind_results);

    ////////
    char id[100],
        title[100],
        event_date[100],
        date_created[100];

    unsigned long stars,subscriptions;

    bool is_null[6];
    bool error[6];
    unsigned long length[6];

    bind_results[3].length = &length[3];
    bind_results[3].error = &error[3];
    bind_results[3].is_null = &is_null[3];
    bind_results[3].buffer_type = MYSQL_TYPE_LONG;

    bind_results[4].length = &length[4];
    bind_results[4].error = &error[4];
    bind_results[4].is_null = &is_null[4];
    bind_results[4].buffer_type = MYSQL_TYPE_LONG;

    bind_results[0].buffer = id;
    bind_results[1].buffer = title;
    bind_results[2].buffer = event_date;
    bind_results[3].buffer = &subscriptions;
    bind_results[4].buffer = &stars;
    bind_results[5].buffer = date_created;

    for (int i = 0; i < 6; i++)
    {
        if(i == 3 || i == 4) continue;

        bind_results[i].length = &length[i];
        bind_results[i].buffer_length = 100;
        bind_results[i].error = &error[i];
        bind_results[i].is_null = &is_null[i];
        bind_results[i].buffer_type = MYSQL_TYPE_STRING;
    }

    if (mysql_stmt_bind_result(stmt, bind_results))
    {
        puts("bind failed");
        goto exit;
    }

    json_object * res = json_object_new_array();

    while (!mysql_stmt_fetch(stmt))
    {
        json_object * this_row = json_object_new_object();

        json_object_object_add(this_row,"id",json_object_new_string(id));
        json_object_object_add(this_row,"title",json_object_new_string(title));
        json_object_object_add(this_row,"event_date",json_object_new_string(event_date));
        json_object_object_add(this_row,"subscriptions",json_object_new_uint64(subscriptions));
        json_object_object_add(this_row,"stars",json_object_new_uint64(stars));
        json_object_object_add(this_row,"last_time",json_object_new_string(date_created));


        json_object_array_add(res,json_object_get(this_row));

        json_object_put(this_row);
    }

    mysql_stmt_close(stmt);
    mysql_close(conn);
    return res;

exit:
    mysql_stmt_close(stmt);
    mysql_close(conn);
    return NULL;
}


