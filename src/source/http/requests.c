#include "http.h"

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

        if(!strcmp(sklow,"sec-websocket-key")){
            map_add(map, sklow, ss);
        }else{

            map_add(map, sklow, sslow);
        }

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

        char *sklow = string_to_lower(sk);

        char *sslow = string_to_lower(ss);

        if(!strcmp(sklow,"sec-websocket-key")){
            map_add(map, sklow, ss);
        }else{

            map_add(map, sklow, sslow);
        }


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
    char res_fmt[] = "HTTP/%s %d %s\r\nContent-Type: %s\r\nAccess-Control-Allow-Origin: *\r\nAccess-Control-Allow-Methods: POST, GET, DELETE, UPDATE, PUT\r\nAccess-Control-Allow-Headers: content-type\r\n\r\n";
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

        if ((send(sock, buff, sizeof buff,0)) < 0)
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
    if ((send(sock, header, strlen(header),0)) == -1)
    {
        return false;
    }

    return true;
}

bool write_404(int sock)
{
    http_res *h_err = malloc(sizeof(http_res));

    h_err->code = 404;
    h_err->code_name = "NOT FOUND";
    h_err->content_length = 30;
    h_err->http_version = "1.1";
    h_err->content_type = "application/json";

    bool bl = write_header(write_http_header_from_struct(h_err), sock);

    char * json_char = "{\"error\":\"Resource not found\"}";

    send(sock,json_char,30,0);

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
    h_err->content_length = 23;
    h_err->http_version = "1.1";
    h_err->content_type = "application/json";

    bool bl = write_header(write_http_header_from_struct(h_err), sock);

    char * json_char = "{\"error\":\"Bad request\"}";

    send(sock,json_char,23,0);

    free(h_err);
    return bl;
}

bool write_json(struct json_object *obj, int sock)
{
    write_OK(sock, "application/json");
    const char *json = json_object_to_json_string_ext(obj,JSON_C_TO_STRING_PLAIN);

    puts(json);

    if (send(sock, json, strlen(json),0) == -1)
    {
        return false;
    }
    return true;
}


void write_ws_accept(int sock,map_t *http_req){
        map_print(http_req);
            bool success = false;
            char *key = createAcceptString(map_get_ref(http_req,"sec-websocket-key"));

            response_builder *rs = response_builder_create();
            response_builder_set_code(rs, "101");
            response_builder_set_status_name(rs, "Switching Protocols");
            response_builder_set_header(rs,"Connection","Upgrade");
            response_builder_set_header(rs,"Upgrade","websocket");

            response_builder_set_header(rs,"Sec-WebSocket-Accept",key);

            char *resp = response_builder_to_string(rs);
            if(resp)
            {
                if(send(sock,resp,strlen(resp),0) > 0){

                    add_to_pfds(&pfds,sock,&fd_count_g,&fd_size_g);
                    success = true;
                }
            }
            
            free(resp);
            response_builder_free(rs);

            if(!success) close(sock);
}