#include "ev_media.h"

json_object * insert_user_image_filename(char *user_id,char *file_name)
{


    char *query = "call insert_user_image(?,?)";

    result_bind * rb = result_bind_create(2);
    result_bind_set_string(0,rb,user_id);
    result_bind_set_string(1,rb,file_name);

    json_object *res =  execute_prepared_call_query(query,rb);

    result_bind_destroy(rb);

    return res;
}

json_object * insert_pub_image_filename(char *pub_id,char *file_name)
{


    char *query = "call insert_pub_image(?,?)";

    result_bind * rb = result_bind_create(2);
    result_bind_set_string(0,rb,pub_id);
    result_bind_set_string(1,rb,file_name);

    json_object *res =  execute_prepared_call_query(query,rb);

    result_bind_destroy(rb);

    return res;
}