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

#define IPV_4 0
#define IPV_6 1
#define BACKLOG 4

void signal_handler(int sig);
void *handle_request(void *args);
void accept_connections(int socketfd);
bool set_up_server();
void clean_up();