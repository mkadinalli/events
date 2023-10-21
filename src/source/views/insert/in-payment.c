
#include "in-payment.h"

void add_payment(SSL *sock, char *json_load)
{

    // write_404(sock);
    json_object *jobj = json_tokener_parse(json_load);
    json_object *phone_number, *amount, *user_id, *published_id;

    if (!json_object_object_get_ex(jobj, "phone", &phone_number))
    {
        write_BAD(sock);
        // todo
        return;
    }

    if (!json_object_object_get_ex(jobj, "amount", &amount))
    {
        write_BAD(sock);
        // todo
        return;
    }

    if (!json_object_object_get_ex(jobj, "user_id", &user_id))
    {
        write_BAD(sock);
        // todo
        return;
    }

    if (!json_object_object_get_ex(jobj, "published_id", &published_id))
    {
        write_BAD(sock);
        // todo
        return;
    }

    char *conv_id = mpesa_do_stk_push(json_object_get_string(phone_number),json_object_get_double(amount));

    if(!conv_id)
    {
        write_BAD(sock);
        //should be server err
        return;
    }

    // write_404(sock);
    json_object *j_res = insert_into_payments(json_object_get_string(user_id),
                              json_object_get_string(published_id),
                              conv_id,
                              json_object_get_float(amount));

    write_json(j_res, sock);
    json_object_put(j_res);
}