#include "payment-da.h"

json_object * insert_into_payments(const char *user_id, const char *pub_id,char *m_id,char *c_id,float amount)
{
    char *query = "call insert_payment (?,?,?,?,?)";

    result_bind * rb = result_bind_create(5);
    result_bind_set_string(0,rb,pub_id);
    result_bind_set_string(1,rb,user_id);
    result_bind_set_float(2,rb,amount);
    result_bind_set_string(3,rb,m_id);
    result_bind_set_string(4,rb,c_id);

    json_object *res =  execute_prepared_call_query(query,rb);

    result_bind_destroy(rb);

    return res;
}