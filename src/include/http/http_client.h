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

char * get_ip_as_string(struct sockaddr *address);

bool http_client_connect();