#pragma once

#include <mysql/mysql.h>
#include <json-c/json.h>
#include <stdio.h>
#include <threads.h>
#include "./lib.h"

void empty();

bool check_if_user_data_exists(const char *username, const char *email);
bool check_if_user_exists(char *identity, char *password, bool by_email);
int find_row_count(char *query);

bool inser_into_users(const char *name,
                      const char *username,
                      const char *email,
                      const char *password);

bool execute_query(char *query);
bool insert_into_pubished(const char *title,
                          const char *description,
                          const char *venue,
                          const char *event_date,
                          const char *deadline_date,
                          const char *publisher_id);

bool insert_into_stars(const char *user_id, const char *publish_id);
bool insert_into_followers(const char *user_id, const char *follower_id);
bool insert_into_subscribers(const char *user_id, const char *publish_id);
json_object *select_from_published(const char *user_id,
                                   const char *last_time,
                                   const char *last_query_time);

json_object *select_one_from_published(const char *id);

bool update_pubished(
                            const char *title,
                          const char *description,
                          const char *venue,
                          const char *event_date,
                          const char *deadline_date,
                          const char *publisher_id,    const char *id);


bool update_one_user(const char *name,
                      const char *username,
                      const char *email,
                      const char * avater,
                      const char * bio,
                      const char * about,
                      const char * id);

json_object *get_user(char *id);

json_object *get_followers_by_id(char *id,char *last_time);// done
                                                              //identical
json_object *get_following_by_id(char *id,char *last_time);//done


json_object *get_published_by_id(char *id,char *last_time);//done


json_object *get_stars_by_user_id(char *id,char *last_time);//done

json_object *get_stars_by_pub_id(char *id,char *last_time); //done




MYSQL *start_db_connection();
