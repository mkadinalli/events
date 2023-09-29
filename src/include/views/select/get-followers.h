#pragma once

#include "../../http/requests.h"
#include "../../lib/ev_strings.h"
#include "../../data-structures/map.h"
#include "../../da/followers-da.h"
#include <string.h>


void get_followers_for_user(int sock,char *url);
void get_followed_by_user(int sock,char *url);