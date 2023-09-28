#include "../../include/da/users-da.h"
#include "../../include/lib/files.h"
#include "../../include/da/db.h"

bool insert_into_users(const char *name, const char *username, const char *email, const char *password)
{
    char *query = "insert into users (name,username,email,pass_word) values (?,?,?,?)";
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

}



json_object *get_user(char *id_)
{
    char *query = "call get_user(?)";
}



