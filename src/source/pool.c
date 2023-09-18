
#include "../include/pool.h"

// creating a work queue
tpool_work_t *tpool_work_create(thread_func_t func, void *arg)
{
    tpool_work_t *work;

    if (func == NULL)
        return NULL;

    work = malloc(sizeof(*work));
    work->func = func;
    work->arg = arg;
    work->next = NULL;
    return work;
}

void tpool_work_destroy(tpool_work_t *work)
{
    if (work == NULL)
        return;
    free(work);
}

tpool_work_t *tpool_work_get(tpool_t *tm)
{
    tpool_work_t *work;

    if (tm == NULL)
        return NULL;

    work = tm->work_first;

    if (work == NULL)
        return NULL;

    if (work->next == NULL)
    {
        tm->work_first = NULL;
        tm->work_last = NULL;
    }
    else
    {
        tm->work_first = work->next;
    }

    return work;
}

int tpool_worker(void *arg)
{
    tpool_t *tm = arg;
    tpool_work_t *work;

    while (1)
    {
        mtx_lock(&(tm->work_mutex));
        while (tm->work_first == NULL && tm->stop == 0)
            cnd_wait(&(tm->work_cond), &(tm->work_mutex));

        if (tm->stop == 1)
            break;

        work = tpool_work_get(tm);
        tm->working_cnt++;

        mtx_unlock(&(tm->work_mutex));

        if (work != NULL)
        {
            work->func(work->arg);
            tpool_work_destroy(work);
        }

        mtx_lock(&(tm->work_mutex));
        tm->working_cnt--;

        if (tm->stop == 0 && tm->working_cnt == 0 && tm->work_first == NULL)
            cnd_signal(&(tm->working_cond));

        mtx_unlock(&(tm->work_mutex));
    }
    tm->thread_cnt--;
    cnd_signal(&(tm->working_cond));

    mtx_unlock(&(tm->work_mutex));

    return 0;
}

tpool_t *tpool_create(size_t num)
{
    tpool_t *tm;
    thrd_t thread;
    size_t i;

    if (num == 0)
        num = 2;

    tm = calloc(1, sizeof(*tm));
    tm->thread_cnt = num;

    mtx_init(&(tm->work_mutex), 0);
    cnd_init(&(tm->work_cond));
    cnd_init(&(tm->working_cond));

    tm->work_first = NULL;
    tm->work_last = NULL;

    for (i = 0; i < num; i++)
    {
        thrd_create(&thread, tpool_worker, tm);
        thrd_detach(thread);
    }

    return tm;
}

void tpool_destroy(tpool_t *tm)
{
    tpool_work_t *work;
    tpool_work_t *work2;

    if (tm == NULL)
        return;

    mtx_lock(&(tm->work_mutex));
    work = tm->work_first;
    while (work != NULL)
    {
        work2 = work->next;
        tpool_work_destroy(work);
        work = work2;
    }

    tm->stop = 1;

    cnd_broadcast(&(tm->work_cond));
    mtx_unlock(&(tm->work_mutex));

    tpool_wait(tm);

    mtx_destroy(&(tm->work_mutex));
    cnd_destroy(&(tm->working_cond));
    cnd_destroy(&(tm->work_cond));

    free(tm);
}

int tpool_add_work(tpool_t *tm, thread_func_t func, void *arg)
{
    tpool_work_t *work;

    if (tm == NULL)
        return -1;

    work = tpool_work_create(func, arg);

    if (work == NULL)
        return -1;

    mtx_lock(&(tm->work_mutex));

    if (tm->work_first == NULL)
    {
        tm->work_first = work;
        tm->work_last = tm->work_first;
    }
    else
    {
        tm->work_last->next = work;
        tm->work_last = work;
    }

    cnd_broadcast(&(tm->work_cond));

    mtx_unlock(&(tm->work_mutex));

    return 0;
}

void tpool_wait(tpool_t *tm)
{
    if (tm == NULL)
        return;

    mtx_lock(&(tm->work_mutex));

    while (1)
    {
        if ((tm->stop == 0 && tm->working_cnt != 0) ||
            (tm->stop == 1 && tm->thread_cnt != 0))
        {
            cnd_wait(&(tm->working_cond), &(tm->work_mutex));
        }
        else
        {
            break;
        }
    }

    mtx_unlock(&(tm->work_mutex));
}
