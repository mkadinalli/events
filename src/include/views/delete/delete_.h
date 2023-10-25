#pragma once

#include "requests.h"
#include "ev_strings.h"
#include "map.h"
#include "users-da.h"
#include "stars-da.h"
#include "events-da.h"
#include "followers-da.h"
#include "subscriptions-da.h"
#include "urls.h"
#include <string.h>

void delete_user(SSL *sock,char *url);

void delete_star(SSL *sock, char *url);

void delete_event(SSL *sock, char *url);

void delete_follower(SSL *sock, char *url);

void delete_subscription(SSL *sock, char *url);
