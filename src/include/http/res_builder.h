#pragma once

#include "data_structures.h"

typedef struct response_builder response_builder;
/*For string responses */
struct response_builder{
    char *status_code;
    char *status_name;
    char *http_version;

    map_t *headers;
    char *body;
};

char *response_builder_to_string(response_builder *res);

bool response_builder_set_header(response_builder *res,char *key,char *value);

bool response_builder_set_code(response_builder *res, char*code);

bool response_builder_set_status_name(response_builder *res, char *status);

bool response_builder_set_body(response_builder *res, char *body);

void response_builder_free(response_builder *res);

response_builder * response_builder_create();



