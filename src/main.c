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

#define BUFFLEN 4096




int main()
{
    char uuid[39];

    uuid_v4_gen(uuid);
    unsigned char * sec_c = string_encrypt("Safaricom999!*!","/home/vic/Desktop/mpesa_crt/pubkey.pem");
    

    char res_fmt[] = "{\
\"OriginatorConversationID\": \"%s\",\
\"InitiatorName\": \"%s\",\
\"SecurityCredential\": \"%s\",\
\"CommandID\": \"%s\",\
\"Amount\": \"%s\",\
\"PartyA\": \"%s\",\
\"PartyB\": \"%s\",\
\"Remarks\": \"%s\",\
\"QueueTimeOutURL\": \"%s\",\
\"ResultURL\": \"%s\",\
\"Occassion\": \"%s\"\
}";


    char res_c[4096];

    sprintf(res_c,res_fmt,
        uuid,
        "testapi",
        sec_c,
        "BusinessPayment",
        "10",
        "600996",
        "254716732614",
        "BusinessPayment",
        "https://mydomain.com/b2c/queue",
        "https://mydomain.com/b2c/queue",
        "Christmass"
    );

    //puts(res_c);

    free(sec_c);

    http_client * cl = http_client_create();

    char * atk = mpesa_get_access_token("pqgen4fQJIx3bSYl17lNYgsBwkY8g44m","5U0icomXgD5mNgkm");

    if(!atk) exit(1);



    string_t * str = string_create_from_string("Bearer ");

    string_concat(str,atk,strlen(atk));

    //puts(str->chars);

    http_client_set_header("Host", "sandbox.safaricom.co.ke", cl);
    http_client_set_header("authorization",str->chars,cl);
    http_client_set_header("Content-Type","application/json",cl);
    http_client_set_url("/mpesa/b2c/v3/paymentrequest",cl);
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

    if(http_client_connect(cl))
    {
        puts(cl->response);
    }
    


    //puts(json_object_to_json_string_ext(req,JSON_C_TO_STRING_PRETTY));
}