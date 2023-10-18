#include "../../../include/views/insert/in-followers.h"

void add_follower(SSL *sock, char *json_load)
{
    // write_404(sock);
    json_object *jobj = json_tokener_parse(json_load);
    json_object *user_id, *follower_id;

    if (!json_object_object_get_ex(jobj, "user_id", &user_id))
    {
        write_BAD(sock);
        // todo
        return;
    }

    if (!json_object_object_get_ex(jobj, "follower_id", &follower_id))
    {
        write_BAD(sock);
        // todo
        return;
    }
    // write_404(sock);

    json_object *j_res = insert_into_followers(json_object_get_string(user_id),
                               json_object_get_string(follower_id));

    write_json(j_res, sock);
    json_object_put(j_res);
}