#include "../../../include/views/insert/in-users.h"
#include "../../../include/da/db.h"
#include "gmail.h"
#include "urls.h"

void sign_up(SSL *sock, char *json_load)
{
    json_object *jobj = json_tokener_parse(json_load);
    json_object *name, *username, *email, *password, *token, *id, *results, *result_one;

    if (!json_object_object_get_ex(jobj, "name", &name))
    {
        write_BAD(sock);
        // todo
        return;
    }

    if (!json_object_object_get_ex(jobj, "username", &username))
    {
        write_BAD(sock);
        // todo
        return;
    }

    if (!json_object_object_get_ex(jobj, "email", &email))
    {
        write_BAD(sock);
        // todo
        return;
    }

    if (!json_object_object_get_ex(jobj, "password", &password))
    {
        write_BAD(sock);
        // todo
        return;
    }

    json_object *j_res = insert_into_users(json_object_get_string(name),
                                           json_object_get_string(username),
                                           json_object_get_string(email),
                                           json_object_get_string(password));

    puts(json_object_to_json_string_ext(j_res,JSON_C_TO_STRING_PRETTY));


    if (!json_object_object_get_ex(j_res, "results", &results))
    {
        write_BAD(sock);
        // todo
        return;
    }

    if ((result_one = json_object_array_get_idx(results, 0)) == NULL)
    {
        write_BAD(sock);
        // todo
        return;
    }

    if (!json_object_object_get_ex(result_one, "token", &token))
    {
        write_BAD(sock);
        // todo
        return;
    }

    if (!json_object_object_get_ex(result_one, "id", &id))
    {
        write_BAD(sock);
        // todo
        return;
    }

    char v_url[200];

    sprintf(v_url, "https://localhost:2000/api/verify/?token=%s&id=%s",
            json_object_get_string(token),
            json_object_get_string(id));

    gmail_send_message(json_object_get_string(email), v_url);
    json_object_put(j_res);


    json_object * rp = json_object_new_object();
    json_object_object_add(rp,"success",json_object_new_boolean(true));

    write_json(rp,sock);
    json_object_put(rp);  
}

void verify_user(SSL *sock, char *url)
{
    char *id = get_param_from_url(url, "id");
    char *last_time_ = get_param_from_url(url, "last_time");

    char *last_time = string_replacechar('@', ' ', last_time_, strlen(last_time_));
    free(last_time_);

    if (id == NULL)
    {
        write_BAD(sock);
        if (last_time)
            free(last_time);
        return;
    }

    if (last_time == NULL)
    {
        write_BAD(sock);
        return;
    }

    json_object *jobj = get_published_by_user_id(id, last_time);

    jobj == NULL ? write_404(sock) : write_json(jobj, sock);

    if (!jobj)
        json_object_put(jobj);
    free(id);
    free(last_time);
}