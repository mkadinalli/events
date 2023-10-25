#include "endpoints.h"

void method_get(SSL *sock, char *url)
{
    if (starts_with_word("/api/login", url))
    {
        login(url, sock);
    }

    if(starts_with_word("/api/user",url))
    {
        get_one_user(sock,url); //done
    }

    if(starts_with_word("/api/followers",url))
    {
        get_followers_for_user(sock,url); //done
    }

    if(starts_with_word("/api/followed",url))
    {
        get_followed_by_user(sock,url); //done
    }

    if(starts_with_word("/api/published",url))
    {
        get_published_by_user(sock,url);// done
    }

    if(starts_with_word("/api/stars",url))
    {
        get_stars_by_user(sock,url); //done
    }

    if(starts_with_word("/api/p-stars",url))
    {
        get_stars_for_publish(sock,url); //done
    }

    if(starts_with_word("/api/p-subscriptions",url))
    {
        get_subs_for_publish(sock,url);//done
    }

    if(starts_with_word("/api/subscriptions",url))
    {
        get_subs_by_user(sock,url);//done
    }

    if(starts_with_word("/api/verify",url))
    {
        verify_user(sock,url);
    }

}

void method_put(SSL *sock, char *url, char *json)
{
    if(starts_with_word("/api/event",url))
    {
        update_event(sock,json);
    }

    if(starts_with_word("/api/user",url))
    {
        update_user(sock,json);
    }
}


void method_post(SSL *sock, char *url, char *json)
{
    if (starts_with_word("/api/signup", url))
    {
        sign_up(sock, json);
    }

    if(starts_with_word("/api/events",url))
    {
        add_event(sock,json);
    }

    if(starts_with_word("/api/follow",url))
    {
        add_follower(sock,json);
    }

    if(starts_with_word("/api/subscribe",url))
    {
        add_subscriber(sock,json);
    }

    if(starts_with_word("/api/star",url))
    {
        add_star(sock,json);
    }

    if(starts_with_word("/api/pay",url))
    {
        add_payment(sock,json);
    }

    if(starts_with_word("/api/gevents",url))
    {
        get_events(sock,json);
    }

    if(starts_with_word("/api/one-event",url))
    {
        get_one_event(sock,json);
    }
}

void method_delete(SSL *sock, char *url){};

void method_post_file(SSL *sock,char *url,char *filename)
{
    if(starts_with_word("/upload/u-image",url))
        insert_user_image(sock,url,filename);

    if(starts_with_word("/upload/p-image",url))
        insert_pub_image(sock,url,filename);
}