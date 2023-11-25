#include "s_message.h"
#include "http.h"

mtx_t message_mutex;
cnd_t message_condition;

messge *message_create(){
    messge *m = malloc(sizeof(messge));
    memset(m,0,sizeof(messge));
    m->message = NULL;
    m->receivers = NULL;
    mtx_init(&message_mutex,0);
    cnd_init(&message_condition);

    return m;
}

bool message_push_back(messge *msg,char *msge, list_t *recs){
    if(!msg){
        return false;
    }
    mtx_lock(&message_mutex);

    if(msg->message == NULL && msg->receivers == NULL){
        msg->receivers = recs;
        msg->message = malloc(strlen(msge) + 1);
        strcpy(msg->message,msge);
        mtx_unlock(&message_mutex);
        return true;
    }
    
    messge *tmp = msg;

    while(tmp->nxt != NULL){
        tmp = tmp->nxt;
    }

    messge *tmp2 = message_create();
    tmp2->receivers = recs;
    tmp2->message = malloc(strlen(msge)+1);
    strcpy(tmp2->message,msge);
    tmp->nxt = tmp2;

    mtx_unlock(&message_mutex);
    cnd_signal(&message_condition);
    return true;
}

bool message_pop_front(messge **msg){
    if(*msg == NULL) return false;

    mtx_lock(&message_mutex);
    if((*msg)->nxt == NULL){
        if((*msg)->message) free((*msg)->message);
        if((*msg)->receivers) list_destroy((*msg)->receivers);
        (*msg)->message = NULL;
        (*msg)->receivers = NULL;
        mtx_unlock(&message_mutex);
        return true;
    }

    messge *tmp = (*msg)->nxt;


    if((*msg)->message) free((*msg)->message);
    if((*msg)->receivers) list_destroy((*msg)->receivers);

    *msg = tmp;
    mtx_unlock(&message_mutex);
    return true;
}

bool messages_is_empty(messge *msg){
    if(msg == NULL) return true;
    return msg->message == NULL;
}

void message_destroy(messge *msg){
    mtx_lock(&message_mutex);
    messge *tmp = msg;

    while(tmp){
        tmp = tmp->nxt;
        
        free(msg->message);
        list_destroy(msg->receivers);

        msg = tmp;
    }
    mtx_unlock(&message_mutex);
}

int start_queue(messge *msg){
    if(msg == NULL){
        exit(1);
    }

    while(1){
        mtx_lock(&message_mutex);
        while(!messages_is_empty(msg))
            cnd_wait(&message_condition,&message_mutex);
        
        for(int i = 0; i < fd_count_g; i++){
            send(pfds[i].fd,msg->message,strlen(msg->message),0);
            
            message_pop_front(msg);
        }
    }
}