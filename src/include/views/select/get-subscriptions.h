#pragma once

#include "../../http/requests.h"
#include "../../lib/ev_strings.h"
#include "../../data-structures/map.h"
#include "../../da/subscriptions-da.h"
#include <strings.h>

/**
 * @param sock socket file descripter
 * @param url url containing id and last fetched time as parameters
* @brief returns first 20 users that have starred the event later than last fetched time
*/
void get_subs_for_publish(SSL *sock,char *url);


void get_subs_by_user(SSL *sock,char *url);