#pragma once

#include "../../http/requests.h"
#include "../../lib/ev_strings.h"
#include "../../data-structures/map.h"
#include "../../da/stars-da.h"

void add_star(int sock,char *json_load);