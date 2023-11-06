#pragma once

#include "../../http/requests.h"
#include "../../lib/ev_strings.h"
#include "../../data-structures/map.h"
#include "../../da/users-da.h"

void sign_up(SSL *sock, char *json_load);

void verify_user(SSL *sock, char *json_load);

void check_username_validity(SSL *sock, char *url);