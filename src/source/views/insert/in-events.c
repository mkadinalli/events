#include "../../../include/views/insert/in-events.h"

void add_event(SSL *sock, char *json_load)
{

    // write_404(sock);
    json_object *jobj = json_tokener_parse(json_load);
    json_object *title, *description, *venue, *event_date, *deadline_date, *publisher_id, *price;

    if (!json_object_object_get_ex(jobj, "title", &title))
    {
        write_BAD(sock);
        // todo
        return;
    }

    if (!json_object_object_get_ex(jobj, "price", &price))
    {
        write_BAD(sock);
        // todo
        return;
    }

    if (!json_object_object_get_ex(jobj, "description", &description))
    {
        write_BAD(sock);
        // todo
        return;
    }

    if (!json_object_object_get_ex(jobj, "venue", &venue))
    {
        write_BAD(sock);
        // todo
        return;
    }

    if (!json_object_object_get_ex(jobj, "event_date", &event_date))
    {
        write_BAD(sock);
        // todo
        return;
    }

    if (!json_object_object_get_ex(jobj, "deadline_date", &deadline_date))
    {
        write_BAD(sock);
        // todo
        return;
    }

    if (!json_object_object_get_ex(jobj, "publisher_id", &publisher_id))
    {
        write_BAD(sock);
        // todo
        return;
    }

    // write_404(sock);
    json_object *j_res = insert_into_published(json_object_get_string(title),
                              json_object_get_string(description),
                              json_object_get_string(venue),
                              json_object_get_string(event_date),
                              json_object_get_string(deadline_date),
                              json_object_get_string(publisher_id),
                              json_object_get_int(price));

    write_json(j_res, sock);
    json_object_put(j_res);
}