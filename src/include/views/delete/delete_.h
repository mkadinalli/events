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

void delete_user(int sock,char *url);

void delete_star(int sock, char *url);

void delete_event(int sock, char *url);

void delete_follower(int sock, char *url);

void delete_subscription(int sock, char *url);
