
#include "in-payment.h"
#include "mpesa.h"

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

    stk_res *sr = mpesa_do_stk_push((char *)json_object_get_string(phone_number),json_object_get_int(amount));

    if(sr == NULL)
    {
        write_BAD(sock);
        //should be server err
        return;
    }

    // write_404(sock);
    json_object *j_res = insert_into_payments(json_object_get_string(user_id),
                              json_object_get_string(published_id),
                              sr->m_id,
                              sr->c_id,
                              json_object_get_int(amount));

    free(sr->c_id);
    free(sr->m_id);
    free(sr);

    write_json(j_res, sock);
    json_object_put(j_res);
}


void process_payment(SSL *sock, char *json_load)
{
    json_object * res = json_tokener_parse(json_load);
    json_object *res_body,*callback_data,*res_code,*m_id,*c_id, *t_id;

    json_object_object_get_ex(res,"Body",&res_body);

    json_object_object_get_ex(res_body,"stkCallback",&callback_data);

    json_object_object_get_ex(callback_data,"ResultCode",&res_code);

    int32_t code = json_object_get_int(res_code);

    if(code != 0){
        json_object_put(res);
        return;
    }

    json_object_object_get_ex(callback_data,"MerchantRequestID",&m_id);
    

    json_object_object_get_ex(callback_data,"CheckoutRequestID",&c_id);

    json_object_object_get_ex(callback_data,"TransactionID",&t_id);


    json_object * v_res = verify_payment(json_object_get_string(m_id),json_object_get_string(c_id),json_object_get_string(t_id));


    json_object_put(res);
    json_object_put(v_res);
}