#pragma once

#include "http.h"

extern struct pollfd *pfds;
extern int fd_count;
extern int fd_size;

#define GUID "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"

bool validate_WS_connection(map_t *request);

char *createAcceptString(char *ws_key);

int createIntFromByte(int *bytes, size_t len);

void *get_in_addr_ws(struct sockaddr *sa);

void add_to_pfds(struct pollfd *pfds[], int newfd, int *fd_count, int *fd_size);

void del_from_pfds(struct pollfd pfds[], int i, int *fd_count);

int startChartSystem(void *v);