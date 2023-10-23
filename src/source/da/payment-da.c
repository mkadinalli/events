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

    printf("Amount is %d\n",amount);

    json_object *res =  execute_prepared_query(query,rb);

    result_bind_destroy(rb);

    return res;
}