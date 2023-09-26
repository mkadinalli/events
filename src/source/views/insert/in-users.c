#include "../../../include/views/insert/in-users.h"
#include "../../../include/da/db.h"


void sign_up(int sock, char *json_load)
{
    // write_404(sock);
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

    // write_404(sock);

    map_t *res = map_create();
    json_object *j_res = NULL;

    if (check_if_user_data_exists(json_object_get_string(username), json_object_get_string(email)))
    {
        map_add(res, "success", "exists");
        j_res = create_json_object_from_map(res);
        write_json(j_res, sock);
        goto clean_up;
    }

    if (!insert_into_users(json_object_get_string(name),
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