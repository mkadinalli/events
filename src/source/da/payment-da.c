#include "payment-da.h"

json_object * insert_into_payments(const char *user_id, const char *pub_id,char *m_id,char *c_id,int amount)
{
    char *query = "insert into payments (user_id,published_id,amount,merchant_request_id,checkout_request_id) values (uuid_to_bin(?),uuid_to_bin(?),?,?,?);";

    result_bind * rb = result_bind_create(5);
    result_bind_set_string(0,rb,user_id);
    result_bind_set_string(1,rb,pub_id);
    result_bind_set_int(2,rb,amount);
    result_bind_set_string(3,rb,m_id);
    result_bind_set_string(4,rb,c_id);

    json_object *res =  execute_prepared_query(query,rb);

    result_bind_destroy(rb);

    return res;
}


json_object *verify_payment(const char *m_id,const char *c_id,const char *t_id)
{
    char *query = "update payments set transaction_id = ? where merchant_request_id = ? and checkout_request_id = ?";

    result_bind * rb = result_bind_create(3);
    result_bind_set_string(0,rb,t_id);
    result_bind_set_string(1,rb,m_id);
    result_bind_set_string(2,rb,c_id);

    json_object *res =  execute_prepared_query(query,rb);

    result_bind_destroy(rb);

    return res;
}