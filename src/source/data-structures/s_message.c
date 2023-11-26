#include "s_message.h"
#include "http.h"
#include "fd_map.h"
#include "db.h"

mtx_t message_mutex;
cnd_t message_condition;

messge *message_create(){
    messge *m = malloc(sizeof(messge));
    memset(m,0,sizeof(messge));
    m->sender_id = NULL;
    m->nxt = NULL;

    mtx_init(&message_mutex,0);
    cnd_init(&message_condition);

    return m;
}

bool message_push_back(messge *msg,char *sender_id){
    if(!msg){
        return false;
    }
    mtx_lock(&message_mutex);

    if(msg->sender_id == NULL){
        msg->sender_id = malloc(strlen(sender_id) + 1);
        strcpy(msg->sender_id,sender_id);
        mtx_unlock(&message_mutex);
        return true;
    }
    
    messge *tmp = msg;

    while(tmp->nxt != NULL){
        tmp = tmp->nxt;
    }

    messge *tmp2 = message_create();
    tmp2->sender_id = malloc(strlen(sender_id)+1);
    strcpy(tmp2->sender_id,sender_id);
    tmp->nxt = tmp2;

    mtx_unlock(&message_mutex);
    cnd_signal(&message_condition);
    return true;
}

bool message_pop_front(messge **msg){
    if(*msg == NULL) return false;

    mtx_lock(&message_mutex);
    if((*msg)->nxt == NULL){
        if((*msg)->sender_id) free((*msg)->sender_id);
        (*msg)->sender_id = NULL;
        mtx_unlock(&message_mutex);
        return true;
    }

    messge *tmp = (*msg)->nxt;


    if((*msg)->sender_id) free((*msg)->sender_id);

    *msg = tmp;
    mtx_unlock(&message_mutex);
    return true;
}

bool messages_is_empty(messge *msg){
    if(msg == NULL) return true;
    return msg->sender_id == NULL;
}

void message_destroy(messge *msg){
    mtx_lock(&message_mutex);
    messge *tmp = msg;

    while(tmp){
        tmp = tmp->nxt;
        
        free(msg->sender_id);

        msg = tmp;
    }
    mtx_unlock(&message_mutex);
}

int start_queue(void *arg){
    messge *msg = (messge *)arg;
    if(msg == NULL){
        exit(1);
    }

    while(1){
        mtx_lock(&message_mutex);
        while(!messages_is_empty(msg))
            cnd_wait(&message_condition,&message_mutex);

        
        char *sender = message_queue->sender_id;
        char *query = "call get_all_followers(?)";
        result_bind *binder = result_bind_create(1);
        result_bind_set_string(0,binder,sender);

        result_bind *followers  = execute_prepared_raw_call_query(query,binder);
        
        while(followers){

            json_object *data_to_send = get_user(followers->value);

            if(data_to_send){
                char *json_data = json_object_to_json_string(data_to_send);

                int receiver_fd = fd_map_get(g_filedescriptor_map,followers->value);

                if(receiver_fd > 0)
                    send(receiver_fd,json_data,strlen(json_data),0);
                
                json_object_put(data_to_send);

            }

            followers = followers->next;
            
        }

        message_pop_front(&msg);
    }
}