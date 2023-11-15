#pragma once

#include <mysql/mysql.h>
#include <json-c/json.h>


#include "../data-structures/ev_string.h"
#include "../data-structures/list.h"
#include "../data-structures/list.h"


struct map_t *
parse_url(char *url);
struct map_t *
parse_url_query(char *query);

char *get_param_from_url(char *url,char *key);

char *get_domain_name_from_url(char *url);

char *get_scheme_from_url(char *url);

char *get_port_from_url(char *url);

char *get_path_from_url(char *url);

bool verify_url(char *url);

char *get_param_part_from_url(char *url);