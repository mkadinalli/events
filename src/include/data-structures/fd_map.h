#pragma once
#include <stdbool.h>
#include "./string.h"
#include "../lib/ev_strings.h"

typedef struct fd_map_t fd_map_t;
struct fd_map_t
{
    int fd;
    char *id;
    fd_map_t *next;
};

fd_map_t *fd_map_create();
bool fd_map_add(fd_map_t *map, int fd, char *id);
bool fd_map_destroy(fd_map_t *map);
void fill_fd_map_item(fd_map_t *map, int fd, char *id);
void fd_map_print(fd_map_t *map);
int fd_map_len(fd_map_t *map);
int fd_map_get(fd_map_t *map, char *id);