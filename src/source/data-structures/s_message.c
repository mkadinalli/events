#include "s_message.h"

messge *message_create(){
    messge *m = malloc(sizeof(messge));
    memset(m,0,sizeof(messge));
    m->message = NULL;
    m->receivers = NULL;

    return m;
}

bool message_push_back(messge *msg,char *msge, list_t *recs){
    if(!msg) return false;

    if(msg->message == NULL && msg->receivers == NULL){
        msg->receivers = recs;
        msg->message = malloc(strlen(msge) + 1);
        strcpy(msg->message,msge);
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
    return true;
}

bool message_pop_front(messge **msg){

    if(*msg == NULL) return false;
    if((*msg)->nxt == NULL){
        if((*msg)->message) free((*msg)->message);
        if((*msg)->receivers) list_destroy((*msg)->receivers);
        free(msg);
        msg = NULL;
        return true;
    }

    messge *tmp = (*msg)->nxt;


    if((*msg)->message) free((*msg)->message);
    if((*msg)->receivers) list_destroy((*msg)->receivers);

    *msg = tmp;
    return true;
}

bool messages_is_empty(messge *msg){
    return msg->message == NULL;
}

void message_destroy(messge *msg){
    messge *tmp = msg;

    while(tmp){
        tmp = tmp->nxt;
        
        free(msg->message);
        list_destroy(msg->receivers);

        msg = tmp;
    }
}