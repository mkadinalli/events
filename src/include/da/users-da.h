#pragma once

#include <mysql/mysql.h>
#include <json-c/json.h>
#include <stdio.h>
#include <threads.h>
#include "../lib/ev_strings.h"
#include "result_bind.h"

/**
 * @brief insert a new user 
*/
json_object * insert_into_users(const char *name,
                      const char *username,
                      const char *email);


/**
 * @brief update one user 
*/
json_object * update_one_user(const char *name,
                      const char * bio,
                      const char * about,
                      const char * id);


/**
 * @brief Get one user by id 
*/
json_object *get_user(char *id);

json_object *verify_user_email(char *id,char *tok,char *password);

json_object *delete_from_users(char *id);

json_object *get_user_by_username(char *id_ );

json_object *get_matching_user(char *id, char *password);