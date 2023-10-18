#include "../../../include/views/insert/in-users.h"
#include "../../../include/da/db.h"


void sign_up(SSL *sock, char *json_load)
{
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
    json_object *j_res = insert_into_users(json_object_get_string(name),
                          json_object_get_string(username),
                          json_object_get_string(email),
                          json_object_get_string(password));

    write_json(j_res, sock);
    json_object_put(j_res);
}