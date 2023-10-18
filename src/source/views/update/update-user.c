#include "../../../include/views/update/update-user.h"


void update_user(SSL *sock,char *json_load)
{
        // write_404(sock);
    json_object *jobj = json_tokener_parse(json_load);
    json_object *name, *username, *avater, *bio, *about, *email,*id;

    if (!json_object_object_get_ex(jobj, "name", &name))
    {
        write_BAD(sock);
        // todo
        puts("mising title");
        return;
    }

    if (!json_object_object_get_ex(jobj, "username", &username))
    {
        write_BAD(sock);
        // todo
        puts("mising desc");
        return;
    }

    if (!json_object_object_get_ex(jobj, "avater", &avater))
    {
        write_BAD(sock);
        // todo
        puts("mising venue");
        return;
    }

    if (!json_object_object_get_ex(jobj, "bio", &bio))
    {
        write_BAD(sock);
        // todo
        puts("mising event date");
        return;
    }

    if (!json_object_object_get_ex(jobj, "about", &about))
    {
        write_BAD(sock);
        // todo
        puts("mising deadline");
        return;
    }

    if (!json_object_object_get_ex(jobj, "email", &email))
    {
        write_BAD(sock);
        // todo
        puts("mising pulisher");
        return;
    }

    if (!json_object_object_get_ex(jobj, "id", &id))
    {
        write_BAD(sock);
        // todo
        puts("mising id");
        return;
    }

    json_object *j_res = update_one_user(json_object_get_string(name),
                              json_object_get_string(username),
                              json_object_get_string(email),
                              json_object_get_string(avater),
                              json_object_get_string(bio),
                              json_object_get_string(about), 
                              json_object_get_string(id));
    write_json(j_res, sock);
    json_object_put(j_res);
}
