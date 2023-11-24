#include "endpoints.h"
#include "http.h"

void method_get_ws(int sock,map_t *req){
    if(!validate_WS_connection(req)){
        close(sock);
        return;
    }

    write_ws_accept(sock,req);
}

void method_get(int sock, char *url)
{
    if (starts_with_word("/files", url))
    {
        upload_file("file", "image/jpeg", sock);
        close(sock);
        return;
    }


    if (starts_with_word("/websock", url))
    {
        upload_file("file", "image/jpeg", sock);
        return;
    }


    if (starts_with_word("/api/login", url))
    {
        login(url, sock);
        close(sock);
        return;
    }

    if(starts_with_word("/api/user",url))
    {
        get_one_user(sock,url); //done
        close(sock);
        return;
    }

    if(starts_with_word("/api/checkuser",url))
    {
        check_username_validity(sock,url); //done
        close(sock);
        return;
    }

    if(starts_with_word("/api/followers",url))
    {
        get_followers_for_user(sock,url); //done
        close(sock);
        return;
    }

    if(starts_with_word("/api/followed",url))
    {
        get_followed_by_user(sock,url); //done
        close(sock);
        return;
    }

    if(starts_with_word("/api/published",url))
    {
        get_published_by_user(sock,url);// done
        close(sock);
        return;
    }

    if(starts_with_word("/api/stars",url))
    {
        get_stars_by_user(sock,url); //done
        close(sock);
        return;
    }

    if(starts_with_word("/api/p-stars",url))
    {
        get_stars_for_publish(sock,url); //done
        close(sock);
        return;
    }

    if(starts_with_word("/api/p-subscriptions",url))
    {
        get_subs_for_publish(sock,url);//done
        close(sock);
        return;
    }

    if(starts_with_word("/api/subscriptions",url))
    {
        get_subs_by_user(sock,url);//done
        close(sock);
        return;
    }

    write_404(sock);
    close(sock);

}

void method_put(int sock, char *url, char *json)
{
    if(starts_with_word("/api/event",url))
    {
        update_event(sock,json);
        close(sock);
        return;
    }

    if(starts_with_word("/api/user",url))
    {
        update_user(sock,json);
        close(sock);
        return;
    }
    write_404(sock);
    close(sock);
}


void method_post(int sock, char *url, char *json)
{

    if (starts_with_word("/api/signup", url))
    {
        sign_up(sock, json);
        close(sock);
        return;
    }

    if(starts_with_word("/api/events",url))
    {
        add_event(sock,json);
        close(sock);
        return;
    }

    if(starts_with_word("/api/follow",url))
    {
        add_follower(sock,json);
        close(sock);
        return;
    }

    if(starts_with_word("/api/subscribe",url))
    {
        add_subscriber(sock,json);
        close(sock);
        return;
    }

    if(starts_with_word("/api/star",url))
    {
        add_star(sock,json);
        close(sock);
        return;
    }

    if(starts_with_word("/api/pay",url))
    {
        add_payment(sock,json);
        close(sock);
        return;
    }

    if(starts_with_word("/api/gevents",url))
    {
        get_events(sock,json);
        close(sock);
        return;
    }

    if(starts_with_word("/api/one-event",url))
    {
        get_one_event(sock,json);
        close(sock);
        return;
    }

    if(starts_with_word("/api/callback",url))
    {
        process_payment(sock,json);
        close(sock);
        return;
    }

    if(starts_with_word("/api/verify",url))
    {
        verify_user(sock,json);
        close(sock);
        return;
    }

    write_404(sock);
    close(sock);
}

void method_delete(int sock, char *url)
{
    if(starts_with_word("/api/subscribe",url))
    {
        delete_subscription(sock,url);
        close(sock);
        return;
    }

    if(starts_with_word("/api/follow",url))
    {
        delete_follower(sock,url);
        close(sock);
        return;
    }

    if(starts_with_word("/api/event",url))
    {
        delete_event(sock,url);
        close(sock);
        return;
    }

    if(starts_with_word("/api/star",url))
    {
        delete_star(sock,url);
        close(sock);
        return;
    }

    if(starts_with_word("/api/user",url))
    {
        delete_user(sock,url);
        close(sock);
        return;
    }

    write_404(sock);
    close(sock);
}

void method_post_file(int sock,char *url,char *content_type,int content_length){

    if(starts_with_word("/upload/u-image",url))
    {
        insert_user_image(sock,url,"temp");
        close(sock);
        return;
    }

    if(starts_with_word("/upload/p-image",url))
    {
        insert_pub_image(sock,url,"temp");
        close(sock);
        return;
    }
}
