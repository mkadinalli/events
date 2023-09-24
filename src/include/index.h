#pragma once

#include "./requests.h"
#include "./lib.h"
#include "./map.h"
#include "./dbc.h"
//10 freaky girls
void login(char *url, int sock);
void sign_up(int sock, char *json_load);
void add_event(int sock,char *json_load);
void add_star(int sock,char *json_load);
void add_subscriber(int sock,char *json_load);
void add_follower(int sock,char *json_load);
void get_events(int sock,char *json_load);
void get_one_event(int sock,char *json_load);
void update_event(int sock,char *json_load);

