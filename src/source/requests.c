#include "../include/requests.h"

char *send_http_request(map_t *map, char *url)
{
    // char *PORT = "3000";
    int socketfd;
    struct addrinfo hints;
    struct addrinfo *server_info, *p;

    memset(&hints, 0, sizeof hints);

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    // 0 for success
    int status = getaddrinfo(url, "2000", &hints, &server_info);

    if (status != 0)
    {
        fprintf(stderr, "Error : %s\n", gai_strerror(status));
        return NULL;
    }

    char address[INET6_ADDRSTRLEN];
    // int port = 0;

    for (p = server_info; p != NULL; p = p->ai_next)
    {

        if ((socketfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
        {
            perror("client socket");
            continue;
        }

        if ((status = connect(socketfd, p->ai_addr, p->ai_addrlen)) == -1)
        {
            close(socketfd);
            perror("client connect");
            continue;
        }

        break;
    }

    if (p == NULL)
    {
        fprintf(stderr, "Failed to connect\n");
        return NULL;
    }

    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
              address,
              sizeof address);

    printf("Connecting to %s\n", address);

    char *message = "GET /song.mp3 HTTP/1.1\r\nContent-Type: text\r\n";

    if ((send(socketfd, message, strlen(message), 0)) == -1)
    {
        perror("send");
        return NULL;
    }

    close(socketfd);
    free(server_info);
    return NULL;
}

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

        char *ss = string_removechar_at(0,
                                        list_get(vc, 1),
                                        strlen(list_get(vc, 1)));

        map_add(map, list_get(vc, 0), ss);

        free(ss);

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

bool upload_file(char *file_name, char *type, int sock)
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

        if ((send(sock, buff, sizeof buff, 0)) < 0)
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

bool write_header(char *header, int sock)
{
    if ((write(sock, header, strlen(header))) == -1)
    {
        // puts(header);
        return false;
    }

    // puts(header);

    return true;
}

bool write_404(int sock)
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

bool write_OK(int sock, char *mime)
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

bool write_BAD(int sock)
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

bool write_json(struct json_object *obj, int sock)
{
    write_OK(sock, "application/json");
    const char *json = json_object_to_json_string(obj);

    if (write(sock, json, strlen(json)) == -1)
    {
        return false;
    }
    return true;
}

//==================================
void serve_JSON(int sock, char *url)
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
        get_stars_for_publish(sock,url);
    }

    if(starts_with_word("/api/p-subscriptions",url))
    {
        get_subs_for_publish(sock,url);
    }

    if(starts_with_word("/api/subscriptions",url))
    {
        get_subs_by_user(sock,url);
    }

}
//==============================

void receive_json(int sock,
                  char *url,
                  char *json)
{
    // {
    //      "name" : "blablabla",
    //      "username" : "......",
    //      "email" : ".......",
    //      "password": "......."
    //  }

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
        puts("yeah man=================");
    }

    if(starts_with_word("/api/u-update",url))
    {
        update_user(sock,json);
    }

}