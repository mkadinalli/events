#include "../../include/da/users-da.h"
#include "../../include/lib/files.h"
#include "../../include/da/db.h"

json_object * insert_into_users(const char *name, const char *username, const char *email)
{
    char *query = "call insert_user(?,?,?)";

    result_bind * rb = result_bind_create(3);
    result_bind_set_string(0,rb,name);
    result_bind_set_string(1,rb,username);
    result_bind_set_string(2,rb,email);
    json_object *res =  execute_prepared_call_query(query,rb);

    result_bind_destroy(rb);

    return res;
}


json_object *verify_user_email(char *id,char *tok,char *password)
{
    char *query = "call verify_user_email(?,?,?)";

    result_bind * rb = result_bind_create(3);
    result_bind_set_string(0,rb,id);
    result_bind_set_string(1,rb,tok);
    result_bind_set_string(2,rb,password);

    json_object *res =  execute_prepared_call_query(query,rb);

    result_bind_destroy(rb);

    return res;
}


json_object * update_one_user(const char *name,
                      const char *username,
                        const char *email,
                            const char * avater,
                                const char * bio,
                                    const char * about,
                                        const char * id)
{
    char *query = "update users set name = ?,username = ?,email = ? ,avater = ?, bio = ? ,about = ? where id = uuid_to_bin(?)";

    result_bind * rb = result_bind_create(7);
    result_bind_set_string(0,rb,name);
    result_bind_set_string(1,rb,username);
    result_bind_set_string(2,rb,email);
    result_bind_set_string(3,rb,avater);
    result_bind_set_string(4,rb,bio);
    result_bind_set_string(5,rb,about);
    result_bind_set_string(6,rb,id);

    json_object *res =  execute_prepared_query(query,rb);

    result_bind_destroy(rb);
    
    return res;
}



json_object *get_user(char *id_)
{
    char *query = "call get_user(?)";

    result_bind * rb = result_bind_create(1);
    result_bind_set_string(0,rb,id_);

    json_object *res =  execute_prepared_call_query(query,rb);

    result_bind_destroy(rb);

    return res;
}


json_object *delete_from_users(char *id)
{
    char *query = "delete from users where id = uuid_to_bin(?)";

    result_bind * rb = result_bind_create(1);
    result_bind_set_string(0,rb,id);

    json_object *res =  execute_prepared_query(query,rb);

    result_bind_destroy(rb);

    return res;
}

json_object *get_user_by_username(char *id_ )
{
    char *query = "call check_username_validity(?)";

    result_bind * rb = result_bind_create(1);
    result_bind_set_string(0,rb,id_);

    json_object *res =  execute_prepared_call_query(query,rb);

    result_bind_destroy(rb);

    return res;
}



