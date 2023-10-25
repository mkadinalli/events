#pragma once

#include <mysql/mysql.h>
#include <json-c/json.h>
#include <stdio.h>
#include <threads.h>
#include "../lib/ev_strings.h"


/**
 * @brief Insert a Star
*/
json_object * insert_into_stars(const char *user_id, const char *publish_id);


/**
 * @brief get 20 events starred by user
*/
json_object *get_stars_by_user_id(char *id,char *last_time);


/**
 * @brief get 20 stars from a published
*/
json_object *get_stars_by_pub_id(char *id,char *last_time);


json_object *delete_from_stars(char *id);