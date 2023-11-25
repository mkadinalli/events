#pragma once

#include "list.h"

typedef struct messge messge;
struct messge{
    char *message;
    list_t *receivers;
    messge *nxt;
};

messge *message_create();
bool message_push_back(messge *msg,char *message, list_t *receivers);
void message_destroy(messge *msg);
bool message_pop_front(messge **msg);