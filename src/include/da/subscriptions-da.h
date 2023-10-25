#pragma once

#include <mysql/mysql.h>
#include <json-c/json.h>
#include <stdio.h>
#include <threads.h>
#include "../lib/ev_strings.h"

/**
 * @brief get 20 subscriptions from a user 
*/
json_object *get_subs_by_user_id(char *id,char *last_time);

/**
 * @brief get 20 subscribers from an event 
*/
json_object *get_subs_by_pub_id(char *id,char *last_time);


/**
 * @brief insert a new subscriber 
*/
json_object * insert_into_subscribers(const char *user_id, const char *publish_id);

json_object *delete_from_subscriptions(char *user_id);