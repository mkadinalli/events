#pragma once

#include <threads.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <threads.h>
#include <netinet/in.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

struct tpool;
typedef struct tpool tpool_t;

typedef int (*thread_func_t)(void *arg);

tpool_t *tpool_create(size_t num);

void tpool_destroy(tpool_t *tm);


int tpool_add_work(tpool_t *tm, thread_func_t func, void *arg);

void tpool_wait(tpool_t *tm);

struct tpool_work
{
    thread_func_t func;
    void *arg;
    struct tpool_work *next;
};

typedef struct tpool_work tpool_work_t;

struct tpool
{

    tpool_work_t *work_first;
    tpool_work_t *work_last;

    mtx_t work_mutex;

    cnd_t work_cond;

    cnd_t working_cond;

    size_t working_cnt;

    size_t thread_cnt;

    int stop;
};

tpool_work_t *tpool_work_get(tpool_t *tm);

int tpool_worker(void *arg);
