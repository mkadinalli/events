#pragma once

#include "../../http/requests.h"
#include "../../lib/ev_strings.h"
#include "../../data-structures/map.h"
#include "../../da/stars-da.h"
#include <string.h>

void get_stars_by_user(int sock,char *url);


/**
 * @param sock socket file descripter
 * @param url url containing id and last fetched time as parameters
 * @brief returns first 20 users that have starred the event later than last fetched time
*/
void get_stars_for_publish(int sock,char *url);