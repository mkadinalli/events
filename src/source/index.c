#include "../include/index.h"

void login(char *url, int sock)
{
    MYSQL *db_conn = NULL;
    db_conn = mysql_init(db_conn);
    db_conn = create_connection_from_a_file(db_conn,
                                            "/home/vic/Desktop/ev2/events/config/config.json");

    if (db_conn == NULL)
    {
        puts("failed to connect to db");
        return;
    }

    puts("connection suuc");

    // mysql_close(db_conn);

    map_t *url_m = parse_url(url);
    if (url_m == NULL)
    {
        write_BAD(sock);
        return;
    }

    if (map_len(url_m) != 2)
    {
        write_BAD(sock);
        map_destroy(url_m);
        return;
    }

    puts(map_get(url_m, "query"));

    map_t *params = parse_url_query(map_get(url_m, "query"));

    puts("parsed");

    if (params == NULL)
    {

        write_BAD(sock);
        return;
    }

    map_print(params);
    puts("-----");

    if (map_len(params) != 2)
    {
        write_BAD(sock);
        map_destroy(url_m);
        return;
    }

    bool log_wit_email = true;
    char *colname = map_get(params, "email");

    if (colname == NULL)
    {
        puts("is nill");
        log_wit_email = false;
        colname = map_get(params, "username");
    }

    puts(colname);

    if (colname == NULL || !map_get(params, "password"))
    {
        write_BAD(sock);
        return;
    }

    puts("connection");

    map_t *res = map_create();
    json_object *jb = NULL;

    puts("json init to null..........");

    if (!check_if_user_exists(db_conn,
                              log_wit_email ? map_get(params, "email") : map_get(params, "username"),
                              map_get(params, "password"),
                              log_wit_email ? true : false))
    {
        map_add(res, "found", "false");

        map_print(res);
        jb = create_json_object_from_map(res);
        write_json(jb, sock);
        goto clean_up;
    }

    map_add(res, "found", "true");
    map_print(res);
    jb = create_json_object_from_map(res);
    write_json(jb, sock);

clean_up:
    mysql_close(db_conn);
    json_object_put(jb);
    map_destroy(params);
    map_destroy(url_m);
    map_destroy(res);
}