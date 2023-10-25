#include "delete_.h"



void delete_user(SSL *sock,char *url)
{

}

void delete_star(SSL *sock, char *url)
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

void delete_event(SSL *sock, char *url)
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

void delete_follower(SSL *sock, char *url)
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

void delete_subscription(SSL *sock, char *url)
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