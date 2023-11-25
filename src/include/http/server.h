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
#include <time.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include "../lib/ev_strings.h"
#include "../../include/threads/pool.h"
#include "../../include/data-structures/map.h"
#include "../../include/data-structures/ev_string.h"
#include "../../include/http/requests.h"
#include "../da/db.h"
#include "s_message.h"


#define IPV_4 0
#define IPV_6 1
#define BACKLOG 4

enum codes
{
    OK = 200,
    NOT_FOUND = 404,
    BAD_REQ = 400
};


typedef enum methods methods;
enum methods
{
    POST = 100,
    GET,
    PUT,
    PATCH,
    DELETE,
    OPTIONS
};

enum files
{
    IMAGE = 10,
    JSON
};

typedef struct ssl_holder ssl_holder;
struct ssl_holder{
    SSL **ssl_strct;
    int sock;
};


void signal_handler(int sig);

int handle_request(void *args);

void accept_connections(int socketfd);

//start server
bool set_up_server(char *port);

void clean_up();

extern int server_fd;
extern SSL_CTX *server_accept_ctx;
extern messge *message_queue;