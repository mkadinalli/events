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

#include "../lib/ev_strings.h"
#include "../../include/threads/pool.h"
#include "../../include/data-structures/map.h"
#include "../../include/data-structures/ev_string.h"
#include "../../include/http/requests.h"


#define IPV_4 0
#define IPV_6 1
#define BACKLOG 4

enum codes
{
    OK = 200,
    NOT_FOUND = 404,
    BAD_REQ = 400
};

enum methods
{
    POST = 100,
    GET,
    PUT,
    PATCH,
    DELETE
};

enum files
{
    IMAGE = 10,
    JSON
};

void signal_handler(int sig);
void *handle_request(void *args);
void accept_connections(int socketfd);

//start server
bool set_up_server(char *port);

void clean_up();