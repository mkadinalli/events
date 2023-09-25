#include "../include/dbc.h"
#include <string.h>

bool check_if_user_data_exists(const char *username, const char *email)
{
    char *queryfmt = "select * from users where email = '%s' or username = '%s'";

    char query[100];

    sprintf(query, queryfmt, email, username);

    int r = find_row_count(query);

    if (r > 0)
        return true;
    return false;
}

bool check_if_user_exists(char *identity, char *password, bool by_email)
{
    char *queryfmt = "call validate_user( %s ,%s ,%s)";

    char *identity_c = string_cover(identity);
    char *password_c = string_cover(password);

    char query[100];

    sprintf(query, queryfmt, password_c, by_email ? identity_c : "null", by_email ? "null" : identity_c);

    free(identity_c);
    free(password_c);

    puts(query);

    if (find_row_count(query) == -1)
        return false;
    return true;
}

int find_row_count(char *query)
{
    MYSQL *conn = NULL;
    conn = mysql_init(conn);
    conn = create_connection_from_a_file(conn,
                                         "/home/vic/Desktop/ev2/events/config/config.json");

    if (conn == NULL)
    {
        puts("failed to connect to db");
        return -1;
    }

    if (mysql_query(conn, query))
    {
        return -1;
    }

    MYSQL_RES *result = NULL;
    result = mysql_store_result(conn);

    if (!result)
        return -1;

    int rows = mysql_num_rows(result);

    mysql_free_result(result);
    mysql_close(conn);

    return rows;
}

bool execute_query(char *query)
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

    if (mysql_query(conn, query))
    {
        return false;
    }

    mysql_close(conn);

    return true;
}

bool inser_into_users(const char *name, const char *username, const char *email, const char *password)
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

    char *query = "insert into users (name,username,email,pass_word) values (?,?,?,?)";

    unsigned long name_l = strlen(name),
                  username_l = strlen(username),
                  email_l = strlen(email),
                  pass_l = strlen(password);

    MYSQL_BIND bind[4];
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
    bind[0].length = &name_l;
    bind[0].buffer_length = 50;
    bind[0].buffer = name;

    bind[1].buffer_type = MYSQL_TYPE_STRING;
    bind[1].is_null = 0;
    bind[1].length = &username_l;
    bind[1].buffer_length = 50;
    bind[1].buffer = username;

    bind[2].buffer_type = MYSQL_TYPE_STRING;
    bind[2].is_null = 0;
    bind[2].length = &email_l;
    bind[2].buffer_length = 50;
    bind[2].buffer = email;

    bind[3].buffer_type = MYSQL_TYPE_STRING;
    bind[3].is_null = 0;
    bind[3].length = &pass_l;
    bind[3].buffer_length = 50;
    bind[3].buffer = password;

    if (mysql_stmt_bind_param(stmt, bind))
    {
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

bool insert_into_pubished(const char *title,
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

bool insert_into_stars(const char *user_id, const char *publish_id)
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

    char *query = "insert into stars (user_id,published_id) values (uuid_to_bin(?),uuid_to_bin(?))";

    unsigned long user_l = strlen(user_id),
                  pub_l = strlen(publish_id);

    MYSQL_BIND bind[2];
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
    bind[0].length = &user_l;
    bind[0].buffer_length = 100;
    bind[0].buffer = user_id;

    bind[1].buffer_type = MYSQL_TYPE_STRING;
    bind[1].is_null = 0;
    bind[1].length = &pub_l;
    bind[1].buffer_length = 100;
    bind[1].buffer = publish_id;

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

bool insert_into_followers(const char *user_id, const char *follower_id)
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

    char *query = "insert into followers (user_id,follower_id) values (uuid_to_bin(?),uuid_to_bin(?))";

    unsigned long user_l = strlen(user_id),
                  fol_l = strlen(follower_id);

    MYSQL_BIND bind[2];
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
    bind[0].length = &user_l;
    bind[0].buffer_length = 100;
    bind[0].buffer = user_id;

    bind[1].buffer_type = MYSQL_TYPE_STRING;
    bind[1].is_null = 0;
    bind[1].length = &fol_l;
    bind[1].buffer_length = 100;
    bind[1].buffer = follower_id;

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

bool insert_into_subscribers(const char *user_id, const char *publish_id)
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

    char *query = "insert into subscriptions (user_id,published_id) values (uuid_to_bin(?),uuid_to_bin(?))";

    unsigned long user_l = strlen(user_id),
                  pub_l = strlen(publish_id);

    MYSQL_BIND bind[2];
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
    bind[0].length = &user_l;
    bind[0].buffer_length = 100;
    bind[0].buffer = user_id;

    bind[1].buffer_type = MYSQL_TYPE_STRING;
    bind[1].is_null = 0;
    bind[1].length = &pub_l;
    bind[1].buffer_length = 100;
    bind[1].buffer = publish_id;

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


bool update_pubished(
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


bool update_one_user(const char *name,
                      const char *username,
                      const char *email,
                      const char * avater,
                      const char * bio,
                      const char * about,
                      const char * id)
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

    char *query = "update users set name = ?,username = ?,email = ? ,avater = ?, bio = ? ,about = ? where id = uuid_to_bin(?)";

    unsigned long name_l = strlen(name),
                  username_l = strlen(username),
                  email_l = strlen(email),
                  av_l = strlen(avater),
                  bio_l = strlen(bio),
                  abt_l = strlen(about),
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
    bind[0].length = &name_l;
    bind[0].buffer_length = 50;
    bind[0].buffer = name;

    bind[1].buffer_type = MYSQL_TYPE_STRING;
    bind[1].is_null = 0;
    bind[1].length = &username_l;
    bind[1].buffer_length = 50;
    bind[1].buffer = username;

    bind[2].buffer_type = MYSQL_TYPE_STRING;
    bind[2].is_null = 0;
    bind[2].length = &email_l;
    bind[2].buffer_length = 50;
    bind[2].buffer = email;


    bind[3].buffer_type = MYSQL_TYPE_STRING;
    bind[3].is_null = 0;
    bind[3].length = &av_l;
    bind[3].buffer_length = 50;
    bind[3].buffer = avater;

    bind[4].buffer_type = MYSQL_TYPE_STRING;
    bind[4].is_null = 0;
    bind[4].length = &bio_l;
    bind[4].buffer_length = 500;
    bind[4].buffer = bio;

    bind[5].buffer_type = MYSQL_TYPE_STRING;
    bind[5].is_null = 0;
    bind[5].length = &abt_l;
    bind[5].buffer_length = 10000;
    bind[5].buffer = about;

    bind[6].buffer_type = MYSQL_TYPE_STRING;
    bind[6].is_null = 0;
    bind[6].length = &id_l;
    bind[6].buffer_length = 50;
    bind[6].buffer = id;


    if (mysql_stmt_bind_param(stmt, bind))
    {
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


json_object *get_user(char *id_)
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

    char *query = "call get_user(?)";

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

    MYSQL_BIND bind_results[14];
    memset(bind_results, 0, sizeof bind_results);

    ///////
    char id[100],
        name[100],
        username[100],
        email[100],
        avater[100],
        about[100],
        bio[100],
        join_date[100];

    long subscribed, stared,followers,following,published,earned_stars;

    bool is_null[14];
    bool error[14];
    unsigned long length[14];
    ///////


    bind_results[8].buffer = &published;
    bind_results[8].length = &length[8];
    bind_results[8].error = &error[8];
    bind_results[8].is_null = &is_null[8];
    bind_results[8].buffer_type = MYSQL_TYPE_LONG;


    bind_results[9].buffer = &followers;
    bind_results[9].length = &length[9];
    bind_results[9].error = &error[9];
    bind_results[9].is_null = &is_null[9];
    bind_results[9].buffer_type = MYSQL_TYPE_LONG;


    bind_results[10].buffer = &following;
    bind_results[10].length = &length[10];
    bind_results[10].error = &error[10];
    bind_results[10].is_null = &is_null[10];
    bind_results[10].buffer_type = MYSQL_TYPE_LONG;

    bind_results[11].buffer = &stared;
    bind_results[11].length = &length[11];
    bind_results[11].error = &error[11];
    bind_results[11].is_null = &is_null[11];
    bind_results[11].buffer_type = MYSQL_TYPE_LONG;

    bind_results[12].buffer = &subscribed;
    bind_results[12].length = &length[12];
    bind_results[12].error = &error[12];
    bind_results[12].is_null = &is_null[12];
    bind_results[12].buffer_type = MYSQL_TYPE_LONG;

    bind_results[13].buffer = &earned_stars;
    bind_results[13].length = &length[13];
    bind_results[13].error = &error[13];
    bind_results[13].is_null = &is_null[13];
    bind_results[13].buffer_type = MYSQL_TYPE_LONG;

    bind_results[0].buffer = id;
    bind_results[1].buffer = name;
    bind_results[2].buffer = username;
    bind_results[3].buffer = email;
    bind_results[4].buffer = avater;
    bind_results[5].buffer = about;
    bind_results[6].buffer = bio;
    bind_results[7].buffer = join_date;

    for (int i = 0; i < 8; i++)
    {
        bind_results[i].length = &length[i];
        bind_results[i].buffer_length = (i > 4 && i<7) ? 1000 : 100;
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
        json_object_object_add(this_row,"name",json_object_new_string(name));
        json_object_object_add(this_row,"username",json_object_new_string(username));
        json_object_object_add(this_row,"email",json_object_new_string(email));
        json_object_object_add(this_row,"avater",json_object_new_string(avater));
        json_object_object_add(this_row,"about",json_object_new_string(about));
        json_object_object_add(this_row,"bio",json_object_new_string(bio));
        json_object_object_add(this_row,"join_date",json_object_new_string(join_date));
        json_object_object_add(this_row,"published",json_object_new_uint64(published));
        json_object_object_add(this_row,"followers",json_object_new_uint64(followers));
        json_object_object_add(this_row,"following",json_object_new_uint64(following));
        json_object_object_add(this_row,"starred",json_object_new_uint64(stared));
        json_object_object_add(this_row,"subscribed",json_object_new_uint64(subscribed));
        json_object_object_add(this_row,"stars_earned",json_object_new_uint64(earned_stars));
        
        puts(name);

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


json_object *get_followers_by_id(char *id_,char *last_time)
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

    char *query = "call get_followers(?,?)";

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

    MYSQL_BIND bind_results[5];
    memset(bind_results, 0, sizeof bind_results);

    ////////
    char id[100],
        user_id[100],
        name[100],
        username[100],
        date_created[100];

    bool is_null[5];
    bool error[5];
    unsigned long length[5];

    bind_results[0].buffer = id;
    bind_results[1].buffer = user_id;
    bind_results[2].buffer = name;
    bind_results[3].buffer = username;
    bind_results[4].buffer = date_created;

    for (int i = 0; i < 5; i++)
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
        json_object_object_add(this_row,"user_id",json_object_new_string(user_id));
        json_object_object_add(this_row,"name",json_object_new_string(name));
        json_object_object_add(this_row,"username",json_object_new_string(username));
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


json_object *get_following_by_id(char *id_,char *last_time)
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

    char *query = "call get_following(?,?)";

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

    MYSQL_BIND bind_results[5];
    memset(bind_results, 0, sizeof bind_results);

    ////////
    char id[100],
        user_id[100],
        name[100],
        username[100],
        date_created[100];

    bool is_null[5];
    bool error[5];
    unsigned long length[5];

    bind_results[0].buffer = id;
    bind_results[1].buffer = user_id;
    bind_results[2].buffer = name;
    bind_results[3].buffer = username;
    bind_results[4].buffer = date_created;

    for (int i = 0; i < 5; i++)
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
        json_object_object_add(this_row,"user_id",json_object_new_string(user_id));
        json_object_object_add(this_row,"name",json_object_new_string(name));
        json_object_object_add(this_row,"username",json_object_new_string(username));
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


json_object *get_published_by_id(char *id_,char *last_time)
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

void empty()
{
    /*MYSQL *conn = NULL;
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
    mysql_close(conn);

    ////////////////////////////////create schema


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

    */
}