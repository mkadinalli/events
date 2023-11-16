#pragma once

#include "urls.h"
#include "map.h"

/*A simple url parser*/
typedef struct url url_t;
struct url{
    char *scheme;
    char *domain;
    char *www_domain;
    char *path;
    char *port;
    map_t * params;
};

url_t *url_parser_parse(char *url);

void url_free(url_t *url);