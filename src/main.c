#include <stdio.h>

#include "urls.h"
#include "url_parser.h"
#include "map.h"
#include "mpesa.h"
#include "web_sock.h"
#include "res_builder.h"
#include "data_structures.h"

#define BUFFLEN 4096



int main()
{

   // set_up_server("2000");

   messge *a = message_create();

   list_t *l = list_create();
   list_pushback(l,"hello");

    list_t *r = list_create();
   list_pushback(r,"hello 2");

    list_t *o = list_create();
   list_pushback(o,"hello 3");

   message_push_back(a,"my message",l);
   message_push_back(a,"Another message",r);
   message_push_back(a,"Another message ofcourse",o);

   message_pop_front(&a);
   message_pop_front(&a);

   //message_destroy(a);


   if(a->receivers != NULL){
        list_print(a->receivers);
   }


   //puts(a->message);
}