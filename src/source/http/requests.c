#include "../../include/http/requests.h"
#include <string.h>
#include <stdlib.h>

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

    list_t *vc = split_lim(' ', vl->value, strlen(vl->value),3);

    if (list_len(vc) != 3)
    {
        fprintf(stderr, "Invalid response\n");
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
    char res_fmt[] = "HTTP/%s %d %s\r\nContent-Type: %s\r\nAccess-Control-Allow-Origin: *\r\nAccess-Control-Allow-Methods: POST, GET, DELETE, UPDATE\r\nAccess-Control-Allow-Headers: content-type\r\n\r\n";
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
        return false;
    }

    return true;
}

bool write_404(SSL *sock)
{
    http_res *h_err = malloc(sizeof(http_res));

    h_err->code = 404;
    h_err->code_name = "NOT FOUND";
    h_err->content_length = 30;
    h_err->http_version = "1.1";
    h_err->content_type = "application/json";

    bool bl = write_header(write_http_header_from_struct(h_err), sock);

    char * json_char = "{\"error\":\"Resource not found\"}";

    SSL_write(sock,json_char,30);

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
    h_err->content_length = 23;
    h_err->http_version = "1.1";
    h_err->content_type = "application/json";

    bool bl = write_header(write_http_header_from_struct(h_err), sock);

    char * json_char = "{\"error\":\"Bad request\"}";

    SSL_write(sock,json_char,23);

    free(h_err);
    return bl;
}

bool write_json(struct json_object *obj, SSL *sock)
{
    write_OK(sock, "application/json");
    const char *json = json_object_to_json_string_ext(obj,JSON_C_TO_STRING_PLAIN);

    puts(json);

    if (SSL_write(sock, json, strlen(json)) == -1)
    {
        return false;
    }
    return true;
}
