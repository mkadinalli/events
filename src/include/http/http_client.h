#pragma once


#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<netinet/in.h>
#include <stdbool.h>
#include "../data-structures/map.h"

typedef struct http_client http_client;
struct http_client
{
    map_t *headers;
    char * url;
    char * http_version;
    char * body;
    size_t file_size;
};


char * get_ip_as_string(struct sockaddr *address);

bool http_client_connect();

http_client *http_client_create();

bool http_client_set_url(char *url,http_client *client);

bool http_client_set_header(char *key,char *value,http_client *client);

bool http_client_append_file(char *path,http_client *client);

bool http_client_append_string(char *str,http_client *client);

