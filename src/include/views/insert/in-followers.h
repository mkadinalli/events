#pragma once

#include "../../http/requests.h"
#include "../../lib/ev_strings.h"
#include "../../data-structures/map.h"
#include "../../da/followers-da.h"

void add_follower(int sock,char *json_load);