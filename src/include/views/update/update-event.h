#pragma once

#include "../../http/requests.h"
#include "../../lib/ev_strings.h"
#include "../../data-structures/map.h"
#include "../../da/events-da.h"

void update_event(int sock,char *json_load);