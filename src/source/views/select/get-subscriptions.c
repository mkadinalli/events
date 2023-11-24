#include "../../../include/views/select/get-subscriptions.h"
#include "../../../include/lib/urls.h"


void get_subs_by_user(int sock,char *url)
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

    json_object * jobj =  get_subs_by_user_id(id,last_time);

    jobj == NULL ? write_404(sock) :  write_json(jobj,sock) ;

    if(!jobj) json_object_put(jobj);
    free(id);
    free(last_time);
}


void get_subs_for_publish(int sock,char *url)
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

    json_object * jobj =  get_subs_by_pub_id(id,last_time);

    jobj == NULL ? write_404(sock) :  write_json(jobj,sock) ;

    if(!jobj) json_object_put(jobj);
    free(id);
    free(last_time);
}
