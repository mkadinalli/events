#include "../../include/da/users-da.h"
#include "../../include/lib/files.h"
#include "../../include/da/db.h"

bool insert_into_users(const char *name, const char *username, const char *email, const char *password)
{
    char *query = "insert into users (name,username,email,pass_word) values (?,?,?,?)";

    result_bind * rb = result_bind_create(6);
    result_bind_set_string(0,rb,name);
    result_bind_set_string(1,rb,username);
    result_bind_set_string(2,rb,email);
    result_bind_set_string(3,rb,password);
    json_object *res =  execute_prepared_query(query,rb);

    result_bind_destroy(rb);

    if(res)
    {
        json_object_put(res);
        return true;
    }

    return false;
}


bool update_one_user(const char *name,
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
    
    if(res)
    {
        json_object_put(res);
        return true;
    }

    return false;
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



