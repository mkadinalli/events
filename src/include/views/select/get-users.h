#pragma once

#include "../../http/requests.h"
#include "../../lib/ev_strings.h"
#include "../../data-structures/map.h"
#include "../../da/users-da.h"
#include "../../lib/json.h"

#include <string.h>

void login(char *url, SSL *sock);
void get_one_user(SSL *sock,char *url);

