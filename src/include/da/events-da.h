
#pragma once

#include <mysql/mysql.h>
#include <json-c/json.h>
#include <stdio.h>
#include <threads.h>
#include "../lib/ev_strings.h"

/**
 * @brief Insert a new event
*/
bool insert_into_published(const char *title,
                          const char *description,
                          const char *venue,
                          const char *event_date,
                          const char *deadline_date,
                          const char *publisher_id);


/**
 * @brief select 20 from Events
*/
json_object *select_from_published(const char *user_id,
                                   const char *last_time,
                                   const char *last_query_time);



/**
 * @brief select 1 from events
*/
json_object *select_one_from_published(const char *id);



/**
 * @brief Update an event
*/
bool update_published(
                            const char *title,
                          const char *description,
                          const char *venue,
                          const char *event_date,
                          const char *deadline_date,
                          const char *publisher_id,    const char *id);


/**
 * @brief Get 20 events published by a user 
*/
json_object *get_published_by_user_id(char *id,char *last_time);//done