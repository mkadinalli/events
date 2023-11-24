#include "../../../include/views/insert/in-users.h"
#include "../../../include/da/db.h"
#include "gmail.h"
#include "urls.h"

void sign_up(int sock, char *json_load)
{
    json_object *jobj = json_tokener_parse(json_load);
    json_object *name, *username, *email, *token, *id, *results, *result_one;

    if (!json_object_object_get_ex(jobj, "fullName", &name))
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

    json_object *j_res = insert_into_users(json_object_get_string(name),
                                           json_object_get_string(username),
                                           json_object_get_string(email));

    if (!json_object_object_get_ex(j_res, "results", &results))
    {
        write_BAD(sock);
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

    sprintf(v_url, "Click on this link to verify <a href=\"http://localhost:3000/verify/%s/%s\">Verify</a>",
            json_object_get_string(token),
            json_object_get_string(id));

    gmail_send_message((char *)json_object_get_string(email), (char *)v_url);
    json_object_put(j_res);

    json_object *rp = json_object_new_object();
    json_object_object_add(rp, "success", json_object_new_boolean(true));

    write_json(rp, sock);
    json_object_put(rp);
}

void verify_user(int sock, char *json_load)
{
    json_object *jobj = json_tokener_parse(json_load);
    json_object *token, *id, *password;

    if (!json_object_object_get_ex(jobj, "token", &token))
    {
        write_BAD(sock);
        // todo
        return;
    }

    if (!json_object_object_get_ex(jobj, "id", &id))
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

    json_object *res = verify_user_email((char *)json_object_get_string(id),
                                         (char *)json_object_get_string(token),
                                         (char *)json_object_get_string(password));

    write_json(res,sock);

    json_object_put(res);
}

void check_username_validity(int sock, char *url)
{

    char *id = get_param_from_url(url, "userName");

    if (id == NULL)
    {
        write_BAD(sock);
        return;
    }

    json_object *jobj = get_user_by_username(id);

    jobj == NULL ? write_404(sock) : write_json(jobj, sock);

    if (!jobj)
        json_object_put(jobj);

    free(id);
}