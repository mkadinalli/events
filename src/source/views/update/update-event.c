#include "../../../include/views/update/update-event.h"

void update_event(int sock,char *json_load)
{

    // write_404(sock);
    json_object *jobj = json_tokener_parse(json_load);
    json_object *title, *description, *venue, *event_date, *deadline_date, *publisher_id,*id;

    if (!json_object_object_get_ex(jobj, "title", &title))
    {
        write_BAD(sock);
        // todo
        puts("mising title");
        return;
    }

    if (!json_object_object_get_ex(jobj, "description", &description))
    {
        write_BAD(sock);
        // todo
        puts("mising desc");
        return;
    }

    if (!json_object_object_get_ex(jobj, "venue", &venue))
    {
        write_BAD(sock);
        // todo
        puts("mising venue");
        return;
    }

    if (!json_object_object_get_ex(jobj, "event_date", &event_date))
    {
        write_BAD(sock);
        // todo
        puts("mising event date");
        return;
    }

    if (!json_object_object_get_ex(jobj, "deadline_date", &deadline_date))
    {
        write_BAD(sock);
        // todo
        puts("mising deadline");
        return;
    }

    if (!json_object_object_get_ex(jobj, "publisher_id", &publisher_id))
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

    json_object *j_res = update_published(json_object_get_string(title),
                              json_object_get_string(description),
                              json_object_get_string(venue),
                              json_object_get_string(event_date),
                              json_object_get_string(deadline_date),
                              json_object_get_string(publisher_id), json_object_get_string(id));

    write_json(j_res, sock);
    json_object_put(j_res);
}
