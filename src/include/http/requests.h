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
#include <openssl/ssl.h>
#include <openssl/err.h>

#include "../data-structures/map.h"
#include "../lib/ev_strings.h"

#include "../views/insert/in-events.h"
#include "../views/insert/in-followers.h"
#include "../views/insert/in-stars.h"
#include "../views/insert/in-subscriptions.h"
#include "../views/insert/in-users.h"
#include "in-payment.h"

#include "../views/select/get-events.h"
#include "../views/select/get-followers.h"
#include "../views/select/get-stars.h"
#include "../views/select/get-subscriptions.h"
#include "../views/select/get-users.h"

#include "../views/update/update-event.h"
#include "../views/update/update-user.h"


#include <string.h>


typedef struct http_t http_res;
struct http_t
{
    int code;
    char *code_name;
    char *content_type;
    int content_length;
    char *http_version;
};

char *
write_http_header_from_struct(http_res *http);
char *
send_http_request(map_t *map,
                  char *url);
void *
get_in_addr(struct sockaddr *sa);
map_t *
parse_http_req(char *req);
map_t * parse_http_response(char *req);
bool upload_file(char *file_name,
                 char *type,
                 SSL *sock);
bool write_header(char *header,
                  SSL *sock);
bool write_404(SSL *sock);
bool write_BAD(SSL *sock);
bool write_OK(SSL *sock, char *mime);
bool write_json(struct json_object *obj, SSL *sock);
void serve_JSON(SSL *sock, char *url);
void receive_json(SSL *sock,
                  char *url,
                  char *json);

void receive_file(SSL *sock,char *url,char *filename);
