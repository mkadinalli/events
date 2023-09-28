#include "ev_media.h"

json_object * insert_user_image_filename(char *user_id,char *file_name)
{


    char *query = "insert into user_images (user_id,file_path) values (uuid_to_bin(?),?)";

    result_bind * rb = result_bind_create(2);
    result_bind_set_string(0,rb,user_id);
    result_bind_set_string(1,rb,file_name);

    json_object *res =  execute_prepared_query(query,rb);

    result_bind_destroy(rb);

    return res;
}