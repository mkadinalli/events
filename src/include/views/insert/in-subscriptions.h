#pragma once

#include "../../http/requests.h"
#include "../../lib/ev_strings.h"
#include "../../data-structures/map.h"
#include "../../da/subscriptions-da.h"

void add_subscriber(int sock,char *json_load);