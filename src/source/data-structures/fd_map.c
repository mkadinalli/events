#include "fd_map.h"
#include <stdio.h>

fd_map_t *fd_map_create()
{
    fd_map_t *t = malloc(sizeof(fd_map_t));
    t->fd = -1;
    t->id = NULL;
    t->next = NULL;

    return t;
}

bool fd_map_add(fd_map_t *map, int fd, char *id)
{
    if (map == NULL)
        return false;

    if (fd_map_len(map) == 0)
    {
        fill_fd_map_item(map, fd, id);
        return true;
    }

    fd_map_t *tmp = map;
    fd_map_t *new_map = fd_map_create();
    while (tmp->next != NULL)
    {
        if (tmp->fd == fd)
        {
            new_map = NULL;
            return false;
        }

        tmp = tmp->next;
    }

    if (tmp->fd == fd)
    {
        new_map = NULL;
        return false;
    }

    fill_fd_map_item(new_map, fd, id);

    tmp->next = new_map;
    return true;
}

int fd_map_len(fd_map_t *list)
{
    if (list == NULL)
        return -1;

    int size = 0;
    fd_map_t *temp_list = list;
    while (temp_list->next != NULL)
    {
        temp_list = temp_list->next;
        size++;
    }

    if (temp_list->id != NULL)
        size++;

    return size;
}

void fd_map_print(fd_map_t *map)
{
    if (map == NULL)
        return;

    fd_map_t *tmp = map;
    while (tmp != NULL)
    {
        printf("%d : %s\n", tmp->fd, tmp->id);
        tmp = tmp->next;
    }
}

void fill_fd_map_item(fd_map_t *map, int fd, char *id)
{
    map->fd = fd;
    map->id = string_create_copy(id);
    map->next = NULL;
}

bool fd_map_destroy(fd_map_t *map)
{
    if (map == NULL)
        return false;

    fd_map_t *tmp = NULL;

    while (map != NULL)
    {
        tmp = map;
        map = map->next;
        free(tmp->id);
        free(tmp);
    }

    return true;
}


int fd_map_get(fd_map_t *map,char *id)
{
    if (fd_map_len(map) < 1)
    {
        return NULL;
    }

    fd_map_t *tmp = map;

    while (tmp != NULL)
    {
        if (!strcmp(id,tmp->id))
        {
            return tmp->fd;
        }
        
        tmp = tmp->next;
    }
    return -1;
}