#include <stdio.h>
#include <json-c/json.h>
#include <assert.h>
#include "./include/http/server.h"
#include <mysql/mysql.h>
#include "lib/files.h"
#include <stdbool.h>
#include "da/db.h"
#include "result_bind.h"
#include <time.h>
#include "events-da.h"
#include "http_client.h"
#include <openssl/ssl.h>
#include "mpesa.h"
#include "gmail.h"
#include <openssl/pem.h>
#include <openssl/err.h>
#include "ev_time.h"

#define BUFFLEN 4096



int main()
{
    //char uuid[39];

    //uuid_v4_gen(uuid);
    //unsigned char * sec_c = string_encrypt("Safaricom999!*!","/home/vic/Desktop/mpesa_crt/pubkey.pem");
    

    char res_fmt[] = "{\
\"BusinessShortCode\": \"%s\",\
\"Password\": \"%s\",\
\"Timestamp\": \"%s\",\
\"TransactionType\": \"%s\",\
\"Amount\": \"%s\",\
\"PartyA\": \"%s\",\
\"PartyB\": \"%s\",\
\"PhoneNumber\": \"%s\",\
\"CallBackURL\": \"%s\",\
\"AccountReference\": \"%s\",\
\"TransactionDesc\": \"%s\"\
}";


    char res_c[4096];

    lipa *lp = mpesa_get_password();

    if(lp == NULL) exit(1);

    sprintf(res_c,res_fmt,
        "174379",
        lp->pass_word,
        lp->time,
        "CustomerPayBillOnline",
        "10",
        "254716732614",
        "174379",
        "254716732614",
        "https://8jjpcdf8-3000.uks1.devtunnels.ms/",
        "BIKESHARE",
        "testing"
    );

    mpesa_destroy_password(lp);

    //puts(res_c);

    //free(sec_c);

    http_client * cl = http_client_create();

    char * atk = mpesa_get_access_token("pqgen4fQJIx3bSYl17lNYgsBwkY8g44m","5U0icomXgD5mNgkm");

    if(!atk) exit(1);



    string_t * str = string_create_from_string("Bearer ");

    string_concat(str,atk,strlen(atk));

    //puts(str->chars);

    http_client_set_header("Host", "sandbox.safaricom.co.ke", cl);
    http_client_set_header("Authorization",str->chars,cl);
    http_client_set_header("Content-Type","application/json",cl);
    http_client_set_url("/mpesa/stkpush/v1/processrequest",cl);
    http_client_set_address("sandbox.safaricom.co.ke",cl);
    http_client_set_method(POST,cl);
    http_client_set_port("443",cl);
    http_client_set_header("Connection", "close", cl);
   

    char c_len[10];

    sprintf(c_len,"%ld",strlen(res_c));

    http_client_set_header("Content-Length",c_len,cl);


    http_client_append_string(res_c,cl);


    if(http_client_connect(cl))
    {
        puts(cl->response);
    }
    


    //puts(json_object_to_json_string_ext(req,JSON_C_TO_STRING_PRETTY));
}