#pragma once

#include "../../http/requests.h"
#include "../../lib/ev_strings.h"
#include "../../data-structures/map.h"
#include "../../da/events-da.h"
#include <string.h>

/**
 * @param sock socket file descripter
 * @param json_load JSON file containing id and last fetched time 
 * @brief returns first 20 events later than last fetched time
*/
void get_events(SSL *sock,char *json_load);

/**
 * @param sock socket file descripter
 * @param json_load JSON file containing id
 * @brief returns an event that matches the given id
*/
void get_one_event(SSL *sock,char *json_load);





void get_published_by_user(SSL *sock,char *url);

