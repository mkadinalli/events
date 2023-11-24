#include "delete_.h"



void delete_user(int sock,char *url)
{
    char * id = get_param_from_url(url,"id");

    if(id == NULL)
    {
        write_BAD(sock);
        return;
    }

    json_object * jobj =  delete_from_users(id);

    jobj == NULL ? write_404(sock) :  write_json(jobj,sock) ;

    if(!jobj) json_object_put(jobj);
    free(id);
}

void delete_star(int sock, char *url)
{
    char * id = get_param_from_url(url,"id");

    if(id == NULL)
    {
        write_BAD(sock);
        return;
    }

    json_object * jobj =  delete_from_stars(id);

    jobj == NULL ? write_404(sock) :  write_json(jobj,sock) ;

    if(!jobj) json_object_put(jobj);
    free(id);
}

void delete_event(int sock, char *url)
{
    char * id = get_param_from_url(url,"id");

    if(id == NULL)
    {
        write_BAD(sock);
        return;
    }

    json_object * jobj =  delete_from_published(id);

    jobj == NULL ? write_404(sock) :  write_json(jobj,sock) ;

    if(!jobj) json_object_put(jobj);
    free(id);

}

void delete_follower(int sock, char *url)
{
    char * id = get_param_from_url(url,"id");

    if(id == NULL)
    {
        write_BAD(sock);
        return;
    }

    json_object * jobj =  delete_from_followers(id);

    jobj == NULL ? write_404(sock) :  write_json(jobj,sock) ;

    if(!jobj) json_object_put(jobj);
    free(id);
}

void delete_subscription(int sock, char *url)
{
    char * id = get_param_from_url(url,"id");

    if(id == NULL)
    {
        write_BAD(sock);
        return;
    }

    json_object * jobj =  delete_from_subscriptions(id);

    jobj == NULL ? write_404(sock) :  write_json(jobj,sock) ;

    if(!jobj) json_object_put(jobj);
    free(id);
}