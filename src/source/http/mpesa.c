#include "mpesa.h"

char *mpesa_get_access_token(char *consumer, char *secret)
{
    http_client *ct = http_client_create();

    http_client_set_method(GET, ct);
    http_client_set_address("sandbox.safaricom.co.ke", ct);
    http_client_set_port("443", ct);
    http_client_set_url("/oauth/v1/generate?grant_type=client_credentials", ct);

    http_client_set_header("Host", "sandbox.safaricom.co.ke", ct);
    http_client_set_header("Cache-control", "no-cache", ct);
    http_client_set_header("Connection", "close", ct);

    // char *a =string_create_mpesa_auth("pqgen4fQJIx3bSYl17lNYgsBwkY8g44m","5U0icomXgD5mNgkm");
    char *a = string_create_mpesa_auth(consumer, secret);

    http_client_set_header("authorization", a, ct);

    //free(a);

    json_object *res = NULL;
    char *token_str = NULL;

    if (http_client_connect(ct))
    {
        if (ct->response)
        {
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

bool mpesa_do_b2c(char * p_number)
{
    char uuid[39];

    uuid_v4_gen(uuid);
    unsigned char * sec_c = string_encrypt("Safaricom999!*!","/home/vic/Desktop/mpesa_crt/pubkey.pem");
    

    char res_fmt[] = "{\n\
    \"OriginatorConversationID\": \"%s\",\n\
    \"InitiatorName\": \"%s\",\n\
    \"SecurityCredential\": \"%s\",\n\
    \"CommandID\": \"%s\",\n\
    \"Amount\": \"%s\",\n\
    \"PartyA\": \"%s\",\n\
    \"PartyB\": \"%s\",\n\
    \"Remarks\": \"%s\",\n\
    \"QueueTimeOutURL\": \"%s\",\n\
    \"ResultURL\": \"%s\",\n\
    \"Occassion\": \"%s\",\n\
    }";


    char res_c[4096];

    sprintf(res_c,res_fmt,
        uuid,
        "testapi",
        sec_c,
        "BusinessPayment",
        "10",
        "600996",
        p_number,
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

    puts(str->chars);
}