#include "../../../include/views/insert/in-subscriptions.h"
#include "../../../include/lib/urls.h"
#include "../../../include/lib/json.h"


void add_subscriber(int sock, char *json_load)
{
    // write_404(sock);
    json_object *jobj = json_tokener_parse(json_load);
    json_object *user_id, *publish_id;

    if (!json_object_object_get_ex(jobj, "user_id", &user_id))
    {
        write_BAD(sock);
        // todo
        return;
    }

    if (!json_object_object_get_ex(jobj, "publish_id", &publish_id))
    {
        write_BAD(sock);
        // todo
        return;
    }
    // write_404(sock);

    json_object *j_res = insert_into_subscribers(json_object_get_string(user_id),
                                 json_object_get_string(publish_id));
    write_json(j_res, sock);
    json_object_put(j_res);
}