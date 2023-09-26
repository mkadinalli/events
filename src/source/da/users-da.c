#include "../../include/da/users-da.h"
#include "../../include/lib/files.h"

bool insert_into_users(const char *name, const char *username, const char *email, const char *password)
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



