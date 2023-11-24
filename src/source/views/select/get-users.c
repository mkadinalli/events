#include "../../../include/views/select/get-users.h"
#include "../../../include/lib/urls.h"
#include "../../../include/da/db.h"


void login(char *url, int sock)
{
    char *password = get_param_from_url(url,"password");
    char * emailOrUsername = get_param_from_url(url,"id");

    if(password == NULL){
        if(emailOrUsername){
            free(emailOrUsername);
        }
        write_BAD(sock);
        return;
    }

    if(emailOrUsername == NULL){
        free(password);
        write_BAD(sock);
        return;
    }


    json_object * res = get_matching_user(emailOrUsername,password);

     res == NULL ? write_404(sock) : write_json(res,sock);

}


void get_one_user(int sock,char *url)
{
    char * id = get_param_from_url(url,"id");

    if (id == NULL)
    {
        write_BAD(sock);
        return;
    }

    json_object * jobj = get_user(id);

    jobj == NULL ? write_404(sock) : write_json(jobj,sock);
}
