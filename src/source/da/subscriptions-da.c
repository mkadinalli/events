#include "../../include/da/subscriptions-da.h"
#include "../../include/lib/files.h"
#include "../../include/da/db.h"

json_object *get_subs_by_user_id(char *id_,char *last_time)
{
    char *query = "call get_subs_for_user(?,?)";

    result_bind * rb = result_bind_create(2);
    result_bind_set_string(0,rb,id_);
    result_bind_set_string(1,rb,last_time);

    json_object *res =  execute_prepared_call_query(query,rb);

    result_bind_destroy(rb);

    return res;
}



json_object *get_subs_by_pub_id(char *id_,char *last_time)
{
    char *query = "call get_subscribers_for_pub(?,?)";

    result_bind * rb = result_bind_create(2);
    result_bind_set_string(0,rb,id_);
    result_bind_set_string(1,rb,last_time);

    json_object *res =  execute_prepared_call_query(query,rb);

    result_bind_destroy(rb);

    return res;
}



bool insert_into_subscribers(const char *user_id, const char *publish_id)
{
    char *query = "insert into subscriptions (user_id,published_id) values (uuid_to_bin(?),uuid_to_bin(?))";

    result_bind * rb = result_bind_create(2);
    result_bind_set_string(0,rb,user_id);
    result_bind_set_string(1,rb,publish_id);

    json_object *res =  execute_prepared_query(query,rb);

    result_bind_destroy(rb);

    if(res)
    {
        json_object_put(res);
        return true;
    }

    return false;

}
