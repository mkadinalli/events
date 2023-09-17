#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <threads.h>
#include <netinet/in.h>
#include <unistd.h>
#include <signal.h>
#include "./map.h"
#include "./lib.h"
#include "./index.h"

typedef struct http_t http_res;
struct http_t {
    int code;
    char * code_name;
    char * content_type;
    int content_length;
    char *http_version;
};

char *write_http_header_from_struct(http_res * http);
char * send_http_request(map_t *map,char * url);
void *get_in_addr(struct sockaddr * sa);
map_t * parse_http_req(char *req);
bool upload_file(char *file_name,char *type, int sock);
bool write_header(char *header,int sock);
bool write_404(int sock);
bool write_BAD(int sock);
bool write_OK(int sock,char *mime);
bool write_json(struct json_object * obj,int sock);
bool serve_JSON(int sock,char *url);
