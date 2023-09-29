#pragma once

#include <mysql/mysql.h>
#include <json-c/json.h>
#include <stdio.h>
#include <threads.h>
#include "../lib/ev_strings.h"

/**
 * @brief get 20 followers for a user
*/
json_object *get_followers_by_user_id(char *id,char *last_time);

/**
 * @brief get 20 users followed by a user
 */                                                            
json_object *get_following_by_user_id(char *id,char *last_time);


/**
 * @brief Insert a new follower
*/
json_object * insert_into_followers(const char *user_id, const char *follower_id);