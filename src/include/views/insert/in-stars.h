#pragma once

#include "../../http/requests.h"
#include "../../lib/ev_strings.h"
#include "../../data-structures/map.h"
#include "../../da/stars-da.h"

void add_star(SSL *sock,char *json_load);