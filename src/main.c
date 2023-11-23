#include <stdio.h>

#include "urls.h"
#include "url_parser.h"
#include "map.h"
#include "mpesa.h"
#include "web_sock.h"
#include "res_builder.h"

#define BUFFLEN 4096



int main()
{
    /*char *message = "fffffffffffffffffffffffffffffffffffffkkkkkkkkkffjjjkffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffferend";
     char result[1024 * 1024];
     int len;
     encode_message(message,strlen(message),true,true,result,&len);

     int fin,opcode,mask,mstart,plen;

     parse_flags(result,&fin,&opcode,&mask);

     printf("fin %d opcode %d mask %d\n",fin,opcode,mask);

     parse_payload_length(result,&plen,&mstart);

     printf("length %d mask_start %d\n",plen,mstart);

    char decoded[1024 * 1024] = {0};

     parse_payload(mstart,plen,NULL,result,decoded);

    for(int i = 0; i < plen; i++){
        printf("[%d] %c\n",i ,decoded[i]);
    }*/
    set_up_server("2000");
}