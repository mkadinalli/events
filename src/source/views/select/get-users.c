#include "../../../include/views/select/get-users.h"
#include "../../../include/lib/urls.h"
#include "../../../include/da/db.h"


void login(char *url, SSL *sock)
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


void get_one_user(SSL *sock,char *url)
{
    char * id = get_param_from_url(url,"id");

    if (id == NULL)
    {
        write_BAD(sock);
        return;
    }

    json_object * jobj = get_user(id);

    jobj == NULL ? write_404(sock) : write_json(jobj,sock);
}
