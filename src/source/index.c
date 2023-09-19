#include "../include/index.h"

void login(char *url, int sock)
{
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

    if (colname == NULL || !map_get(params, "password"))
    {
        write_BAD(sock);
        return;
    }

    map_t *res = map_create();
    json_object *jb = NULL;

    if (!check_if_user_exists(
            log_wit_email ? map_get(params, "email") : map_get(params, "username"),
            map_get(params, "password"),
            log_wit_email ? true : false))
    {
        map_add(res, "found", "false");
        jb = create_json_object_from_map(res);
        write_json(jb, sock);
        goto clean_up;
    }

    map_add(res, "found", "true");
    jb = create_json_object_from_map(res);
    write_json(jb, sock);

clean_up:
    json_object_put(jb);
    map_destroy(params);
    map_destroy(url_m);
    map_destroy(res);
}

void sign_up(int sock, char *json_load)
{
    //write_404(sock);
    json_object *jobj = json_tokener_parse(json_load);
    json_object *name, *username, *email, *password;

    if (!json_object_object_get_ex(jobj, "name", &name))
    {
        write_BAD(sock);
        // todo
        return;
    }

    if (!json_object_object_get_ex(jobj, "username", &username))
    {
        write_BAD(sock);
        // todo
        return;
    }

    if (!json_object_object_get_ex(jobj, "email", &email))
    {
        write_BAD(sock);
        // todo
        return;
    }

    if (!json_object_object_get_ex(jobj, "password", &password))
    {
        write_BAD(sock);
        // todo
        return;
    }

    //write_404(sock);

    map_t *res = map_create();
    json_object *j_res = NULL;

    if (check_if_user_data_exists(json_object_get_string(username), json_object_get_string(email)))
    {
        map_add(res, "success", "exists");
        j_res = create_json_object_from_map(res);
        write_json(j_res, sock);
        goto clean_up;
    }

    if (!inser_into_users(json_object_get_string(name),
                          json_object_get_string(username),
                          json_object_get_string(email),
                          json_object_get_string(password)))
    {
        map_add(res, "success", "false");
        j_res = create_json_object_from_map(res);
        write_json(j_res, sock);
        goto clean_up;
    }

    map_add(res, "success", "true");
    j_res = create_json_object_from_map(res);
    write_json(j_res, sock);

clean_up:
    json_object_put(j_res);
}