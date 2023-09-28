#include "../../include/da/events-da.h"
#include "../../include/lib/files.h"
#include "../../include/da/db.h"

bool insert_into_published(const char *title,
                          const char *description,
                          const char *venue,
                          const char *event_date,
                          const char *deadline_date,
                          const char *publisher_id)
{

    char *query = "insert into published (title,description,venue,event_date,deadline_date,publisher_id) values (?,?,?,?,?,uuid_to_bin(?))";
    
    result_bind * rb = result_bind_create(6);
    result_bind_set_string(0,rb,title);
    result_bind_set_string(1,rb,description);
    result_bind_set_string(2,rb,venue);
    result_bind_set_string(3,rb,event_date);
    result_bind_set_string(4,rb,deadline_date);
    result_bind_set_string(5,rb,publisher_id);

    json_object *res =  execute_prepared_query(query,rb);

    result_bind_destroy(rb);

    return res;
}


json_object *select_from_published(const char *user_id,
                                   const char *last_time,
                                   const char *last_query_time)
{
    char *query = "call get_many_published(? ,? ,?)";

    result_bind * rb = result_bind_create(3);
    result_bind_set_string(0,rb,user_id);
    result_bind_set_string(1,rb,last_time);
    result_bind_set_string(2,rb,last_query_time);

    json_object *res =  execute_prepared_call_query(query,rb);

    result_bind_destroy(rb);
    
    return res;


}


json_object *select_one_from_published(const char *id_)
{
    char *query = "call get_one_published(?)";

    result_bind * rb = result_bind_create(1);
    result_bind_set_string(0,rb,id_);

    json_object *res =  execute_prepared_call_query(query,rb);

    result_bind_destroy(rb);
    
    return res;
}


bool update_published(
                            const char *title,
                          const char *description,
                          const char *venue,
                          const char *event_date,
                          const char *deadline_date,
                          const char *publisher_id,     const char *id)
{
    char *query = "update published set title = ?,description = ?,venue = ? ,event_date = ?,deadline_date = ?,publisher_id = uuid_to_bin(?) where id = uuid_to_bin(?)";
    result_bind * rb = result_bind_create(7);
    result_bind_set_string(0,rb,title);
    result_bind_set_string(1,rb,description);
    result_bind_set_string(2,rb,venue);
    result_bind_set_string(3,rb,event_date);
    result_bind_set_string(4,rb,deadline_date);
    result_bind_set_string(5,rb,publisher_id);
    result_bind_set_string(6,rb,id);

    json_object *res =  execute_prepared_query(query,rb);

    result_bind_destroy(rb);
    
    return res;
}


json_object *get_published_by_user_id(char *id_,char *last_time)
{
    result_bind * rb = result_bind_create(2);
    result_bind_set_string(0,rb,id_);
    result_bind_set_string(1,rb,id_);

    char *query = "call get_pub_for_user(?,?)";

    json_object *res =  execute_prepared_call_query(query,rb);

    result_bind_destroy(rb);
    
    return res;
}


