#pragma once

#include "list.h"
#include <threads.h>

extern mtx_t message_mutex;
extern cnd_t message_condition;

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

bool messages_is_empty(messge *msg);

int start_queue(void *arg);