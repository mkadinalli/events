#include "../../include/http/requests.h"
#include <string.h>
#include <stdlib.h>
#include "in_images.h"

void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
    {
        return &(((struct sockaddr_in *)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

map_t *parse_http_req(char *req)
{
    int len = strlen(req);
    req = string_removechar('\r', req, len);
    list_t *lines = split('\n', req, strlen(req));
    list_popback(lines);
    list_popback(lines);

    list_t *vl = lines;

    list_t *vc = split(' ', vl->value, strlen(vl->value));

    if (list_len(vc) != 3)
    {
        fprintf(stderr, "Invalid request\n");
        return NULL;
    }

    map_t *map = map_create();

    map_add(
        map,
        "method",
        list_get(vc, 0));

    map_add(
        map,
        "url",
        list_get(vc, 1));

    map_add(
        map,
        "Http-Version",
        list_get(vc, 2));

    vl = vl->next;

    list_destroy(vc);

    while (vl != NULL)
    {
        vc = split_lim(':', vl->value, strlen(vl->value), 2);
        if (list_len(vc) != 2)
        {
            list_destroy(vc);
            vl = vl->next;
            continue;
        }

        char *ss = remove_leading_and_trailing_spaces(list_get(vc, 1));
        char *sk = remove_leading_and_trailing_spaces(list_get(vc, 0));

        char *sslow = string_to_lower(ss);
        char *sklow = string_to_lower(sk);

        map_add(map, sklow, sslow);

        free(sslow);
        free(sklow);
        free(ss);
        free(sk);

        list_destroy(vc);
        vl = vl->next;
    }

    free(req);
    return map;
}


map_t * parse_http_response(char *req)
{
    int len = strlen(req);
    req = string_removechar('\r', req, len);
    list_t *lines = split('\n', req, strlen(req));
    list_popback(lines);
    list_popback(lines);

    list_t *vl = lines;

    list_t *vc = split(' ', vl->value, strlen(vl->value));

    if (list_len(vc) != 3)
    {
        fprintf(stderr, "Invalid request\n");
        return NULL;
    }

    map_t *map = map_create();

    map_add(
        map,
        "http-version",
        list_get(vc, 0));

    map_add(
        map,
        "status-code",
        list_get(vc, 1));

    map_add(
        map,
        "code-name",
        list_get(vc, 2));

    vl = vl->next;

    list_destroy(vc);

    while (vl != NULL)
    {
        vc = split_lim(':', vl->value, strlen(vl->value), 2);
        if (list_len(vc) != 2)
        {
            list_destroy(vc);
            vl = vl->next;
            continue;
        }

        char *ss = remove_leading_and_trailing_spaces(list_get(vc, 1));
        char *sk = remove_leading_and_trailing_spaces(list_get(vc, 0));

        char *sslow = string_to_lower(ss);
        char *sklow = string_to_lower(sk);

        map_add(map, sklow, sslow);

        free(sslow);
        free(sklow);
        free(ss);
        free(sk);

        list_destroy(vc);
        vl = vl->next;
    }

    free(req);
    return map;

}

char *write_http_header_from_struct(http_res *http)
{
    char res_fmt[] = "HTTP/%s %d %s\r\nContent-Type: %s\r\n\r\n";
    char res[500];

    sprintf(res,
            res_fmt,
            http->http_version,
            http->code,
            http->code_name,
            http->content_type);

    return string_create_from_string(res)->chars;
}

bool upload_file(char *file_name, char *type, SSL *sock)
{
    bool success = true;

    FILE *myfile;
    myfile = fopen(file_name, "rb");

    if (myfile == NULL)
    {
        write_404(sock);
        return false;
    }

    write_OK(sock, type);

    char buff[100] = {0};
    while (!feof(myfile))
    {
        fread(buff, 1, sizeof buff, myfile);

        if ((SSL_write(sock, buff, sizeof buff)) < 0)
        {
            // perror("send");
            success = false;
            break;
        }

        bzero(buff, sizeof buff);
    }

    fclose(myfile);

    return success;
}

bool write_header(char *header, SSL *sock)
{
    if ((SSL_write(sock, header, strlen(header))) == -1)
    {
        // puts(header);
        return false;
    }

    // puts(header);

    return true;
}

bool write_404(SSL *sock)
{
    http_res *h_err = malloc(sizeof(http_res));

    h_err->code = 404;
    h_err->code_name = "NOT FOUND";
    h_err->content_length = 0;
    h_err->http_version = "1.1";
    h_err->content_type = "NULL";

    bool bl = write_header(write_http_header_from_struct(h_err), sock);

    free(h_err);
    return bl;
}

bool write_OK(SSL *sock, char *mime)
{
    http_res *hp = malloc(sizeof(http_res));

    hp->code = 200;
    hp->code_name = "OK";
    hp->content_length = 0;
    hp->content_type = mime;
    hp->http_version = "1.1";

    bool b = write_header(write_http_header_from_struct(hp), sock);
    free(hp);
    return b;
}

bool write_BAD(SSL *sock)
{
    http_res *h_err = malloc(sizeof(http_res));

    h_err->code = 400;
    h_err->code_name = "BAD REQUEST";
    h_err->content_length = 0;
    h_err->http_version = "1.1";
    h_err->content_type = "NULL";

    bool bl = write_header(write_http_header_from_struct(h_err), sock);

    free(h_err);
    return bl;
}

bool write_json(struct json_object *obj, SSL *sock)
{
    write_OK(sock, "application/json");
    const char *json = json_object_to_json_string(obj);

    if (SSL_write(sock, json, strlen(json)) == -1)
    {
        return false;
    }
    return true;
}

//==================================
void serve_JSON(SSL *sock, char *url)
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

    if(starts_with_word("/api/u-followers",url))
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
//==============================

void receive_json(SSL *sock,
                  char *url,
                  char *json)
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

    if(starts_with_word("/api/gevents",url))
    {
        get_events(sock,json);
    }

    if(starts_with_word("/api/one-event",url))
    {
        get_one_event(sock,json);
    }

    if(starts_with_word("/api/ev-update",url))
    {
        update_event(sock,json);
    }

    if(starts_with_word("/api/u-update",url))
    {
        update_user(sock,json);
    }

    if(starts_with_word("/api/pay",url))
    {
        add_payment(sock,json);
    }

}

void receive_file(SSL *sock,char *url,char *filename)
{
    if(starts_with_word("/upload/u-image",url))
        insert_user_image(sock,url,filename);

    if(starts_with_word("/upload/p-image",url))
        insert_pub_image(sock,url,filename);
}