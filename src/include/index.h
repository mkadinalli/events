#pragma once

#include "./requests.h"
#include "./lib.h"
#include "./map.h"
#include "./dbc.h"


void login(char *url, int sock);
void sign_up(int sock, char *json_load);
void add_event(int sock,char *json_load);
void add_star(int sock,char *json_load);
void add_subscriber(int sock,char *json_load);
void add_follower(int sock,char *json_load);
void get_events(int sock,char *json_load);
void get_one_event(int sock,char *json_load);
void update_event(int sock,char *json_load);
void update_user(int sock,char *json_load);
void get_one_user(int sock,char *url);

void get_followers_for_user(int sock,char *url);//done

void get_followed_by_user(int sock,char *url);//done

void get_published_by_user(int sock,char *url);//done

void get_stars_by_user(int sock,char *url); //done


void get_subs_by_user(int sock,char *url);//done

void get_stars_for_publish(int sock,char *url);//done

void get_subs_for_publish(int sock,char *url);


