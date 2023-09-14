#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
//#include <threads.h>
#include <netinet/in.h>
#include <unistd.h>
#include <signal.h>

#include "./map.h"
char *write_http_request_from_map(map_t * map);
char * send_http_request(map_t *map,char * url);
void *get_in_addr(struct sockaddr * sa);