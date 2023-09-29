#include "../../../include/views/select/get-events.h"
#include "../../../include/lib/urls.h"

void get_events(int sock, char *json_load)
{
    // write_404(sock);
    json_object *jobj = json_tokener_parse(json_load);
    json_object *res = NULL;
    json_object *user_id, *last_time, *last_query_time;

    if (!json_object_object_get_ex(jobj, "user_id", &user_id))
    {
        write_BAD(sock);
        // todo
        return;
    }

    if (!json_object_object_get_ex(jobj, "last_time", &last_time))
    {
        write_BAD(sock);
        // todo
        return;
    }

    if (!json_object_object_get_ex(jobj, "last_query_time", &last_query_time))
    {
        write_BAD(sock);
        // todo
        return;
    }

    res = select_from_published(json_object_get_string(user_id),
                                json_object_get_string(last_time),
                                json_object_get_string(last_query_time));

    if (res)
    {
        write_json(res, sock);
        json_object_put(res);
    }
    else
    {
        write_404(sock);
    };

    json_object_put(jobj);
}

void get_one_event(int sock, char *json_load)
{
    json_object *jobj = json_tokener_parse(json_load);
    json_object *res = NULL;
    json_object *id;

    if (!json_object_object_get_ex(jobj, "id", &id))
    {
        write_BAD(sock);
        // todo
        return;
    }

    res = select_one_from_published(json_object_get_string(id));

    if (res)
    {
        write_json(res, sock);
        json_object_put(res);
    }
    else
    {
        write_404(sock);
    };

    json_object_put(jobj);
}


void get_published_by_user(int sock,char *url)
{
    char * id = get_param_from_url(url,"id");
    char * last_time_ = get_param_from_url(url,"last_time");

    char *last_time = string_replacechar('@',' ',last_time_,strlen(last_time_));
    free(last_time_);

    if(id == NULL)
    {
        write_BAD(sock);
        if(last_time) free(last_time);
        return;
    }

    if(last_time == NULL)
    {
        write_BAD(sock);
        return;
    }

    json_object * jobj =  get_published_by_user_id(id,last_time);

    jobj == NULL ? write_404(sock) :  write_json(jobj,sock) ;

    if(!jobj) json_object_put(jobj);
    free(id);
    free(last_time);
}