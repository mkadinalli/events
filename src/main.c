#include <stdio.h>
#include "./include/string_buffer.h"
#include "./include/list.h"

int 
main () 
{

    list_t * ls = list_create();
    list_pushfront(&ls,"hello1");
    list_pushfront(&ls,"hello2");
    list_pushfront(&ls,"hello");
    //list_pushfront(ls,"hello");
    list_popfront(&ls);
    list_popfront(&ls);
    //list_popfront(ls);
    //printf("%s\n",list_back(ls));
    list_print(ls);
    list_destroy(ls);
    /*string_t *exp = string_create_from_string("hel");

    string_append(exp,'a');
    string_append(exp,'b');
    string_append(exp,'c');

    printf("%s\n",exp->chars);

    string_destroy(exp);*/

    return 0;
}
