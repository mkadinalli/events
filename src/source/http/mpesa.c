#include "mpesa.h"
#include "ev_time.h"

char *mpesa_get_access_token(char *consumer, char *secret)
{
    http_client *ct = http_client_create();

    http_client_set_method(GET, ct);
    http_client_set_url("https://sandbox.safaricom.co.ke/oauth/v1/generate?grant_type=client_credentials", ct);
    //http_client_set_header("Cache-Control", "no-cache", ct);
    http_client_set_header("Connection", "close", ct);
    //http_client_set_header("User-Agent", "My-C-proj v0.0.1", ct);

    // char *a =string_create_mpesa_auth("pqgen4fQJIx3bSYl17lNYgsBwkY8g44m","5U0icomXgD5mNgkm");
    char *a = string_create_mpesa_auth(consumer, secret);

    http_client_set_header("Authorization", a, ct);

    //free(a);

    json_object *res = NULL;
    char *token_str = NULL;

    if (http_client_connect(ct))
    {
        map_print(ct->response_headers);
        if (ct->response)
        {
            //puts(ct->response);

            

            if ((res = json_tokener_parse(ct->response)) != NULL)
            {
                json_object *token = NULL;
                if (json_object_object_get_ex(res, "access_token", &token))
                {
                    const char *tval = json_object_get_string(token);
                    token_str = malloc(strlen(tval) + 1);
                    strcpy(token_str, tval);
                    //json_object_put(res);
                    json_object_put(token);
                }
            }
        }
    }

    //free client

    http_client_destroy(ct);

    return token_str;
    //return NULL;
}

stk_res *mpesa_do_stk_push(char * p_number,int amount)
{
    char res_fmt[] = "{\
\"BusinessShortCode\": \"%s\",\
\"Password\": \"%s\",\
\"Timestamp\": \"%s\",\
\"TransactionType\": \"%s\",\
\"Amount\": \"%d\",\
\"PartyA\": \"%s\",\
\"PartyB\": \"%s\",\
\"PhoneNumber\": \"%s\",\
\"CallBackURL\": \"%s\",\
\"AccountReference\": \"%s\",\
\"TransactionDesc\": \"%s\"\
}";


    char res_c[4096];

    lipa *lp = mpesa_get_password();

    if(lp == NULL) return NULL;

    sprintf(res_c,res_fmt,
        "174379",
        lp->pass_word,
        lp->time,
        "CustomerPayBillOnline",
        amount,
        p_number,
        "174379",
        p_number,
        "https://a9b8-105-167-22-94.ngrok.io/api/callback",
        "EV",
        "testing"
    );

    mpesa_destroy_password(lp);

    http_client * cl = http_client_create();

    char * atk = mpesa_get_access_token("Gqw8dLd1qCKl11AXA2Wf2YxoDtMwtayj","65RuQFXbGAALIsKU");

    if(!atk) return NULL;



    string_t * str = string_create_from_string("Bearer ");

    string_concat(str,atk,strlen(atk));

    puts(str->chars);

    http_client_set_header("Authorization",str->chars,cl);
    http_client_set_header("Content-Type","application/json",cl);
    http_client_set_url("https://sandbox.safaricom.co.ke/mpesa/stkpush/v1/processrequest",cl);
    http_client_set_method(POST,cl);
    http_client_set_header("Connection", "close", cl);
    http_client_set_header("User-Agent", "My-C-proj v0.0.1", cl);
   

    char c_len[10];

    sprintf(c_len,"%ld",strlen(res_c));

    http_client_set_header("Content-Length",c_len,cl);


    http_client_append_string(res_c,cl);

    json_object *res = NULL;
    char *token_str = NULL;
    char *token_str2 = NULL;

    if (http_client_connect(cl))
    {
        if (cl->response)
        {

            if ((res = json_tokener_parse(cl->response)) != NULL)
            {
                json_object *token = NULL;
                json_object *token2 = NULL;
                if (json_object_object_get_ex(res, "MerchantRequestID", &token))
                {
                    const char *tval = json_object_get_string(token);
                    token_str = malloc(strlen(tval) + 1);
                    strcpy(token_str, tval);
                    //json_object_put(res);
                    json_object_put(token);
                }

                if (json_object_object_get_ex(res, "CheckoutRequestID", &token2))
                {
                    const char *tval = json_object_get_string(token2);
                    token_str2 = malloc(strlen(tval) + 1);
                    strcpy(token_str2, tval);
                    //json_object_put(res);
                    json_object_put(token2);
                }
            }
        }
    }

    http_client_destroy(cl);

    if(token_str2 == NULL || token_str == NULL) return NULL;

     stk_res * results = malloc(sizeof(stk_res));

     results->c_id = token_str2;
     results->m_id = token_str;
    
    return results;
}


 lipa * mpesa_get_password()
 {
    char short_code[500] =  "174379";
    char lipa_time[100];
    get_current_time(lipa_time);

    char * passkey = "bfb279f9aa9bdbcf158e97dd71a467cd2e0c893059b10f78e6b72ada1ed2c919";

    strcat(short_code,passkey);
    strcat(short_code,lipa_time);

    unsigned char * enc_pass = base64_encode((unsigned char *)short_code,strlen(short_code));

    lipa *l = malloc(sizeof(lipa));

    l->pass_word = enc_pass;
    l->time = malloc(strlen(lipa_time)+1);
    strcpy(l->time,lipa_time);

    return l;
 }

 void mpesa_destroy_password(lipa *l)
 {
    if(!l) return;
    if(l->pass_word)free(l->pass_word);
    if(l->time)free(l->time);
    free(l);
 }