#include "../include/list.h"
#include <assert.h>

list_t *
list_create()
{
    list_t* t = malloc(sizeof(list_t));
    t->next = NULL;
    t->value = NULL;
    return t;
}

bool 
list_pushback(list_t * list,char *str)
{
    if(list == NULL)
        return false;

    if(list_len(list) == 0)
    {
        fill_list_item(list,str);
        return true;
    }

    list_t * temp_list = list;
    while(temp_list->next != NULL)
    {
        temp_list = temp_list->next;
    }

    list_t *new_list = list_create();

    fill_list_item(new_list,str);
    temp_list->next = new_list;
    return true; 


}

bool 
list_pushfront(list_t **list,char * str)
{
    if(*list == NULL)
        return false;
    
    if(list_len(*list) == 0)
    {
        fill_list_item(*list,str);
        return true;
    }

    list_t *new_list = list_create();

    fill_list_item(new_list,str);
    new_list->next = *list;
    *list = new_list;
    return true; 
}

unsigned int 
list_len(list_t *list)
{
    if(list == NULL)
        return -1;

    int size = 0;
    list_t * temp_list = list;
    while(temp_list->next != NULL)
    {
        temp_list = temp_list->next;
        size++;
    }

    if(temp_list->value != NULL)
        size++;

    return size;
}

void 
fill_list_item(list_t *temp_list,char *str)
{
        string_t *string_builder = string_create_from_string(str);
        int string_len = strlen(string_builder->chars)+1;
        temp_list->value = malloc(sizeof(char) * string_len);
        strcpy(temp_list->value,string_builder->chars);
        temp_list->value[string_len] = '\0';
        temp_list->next = NULL;

        free(string_builder);
}

void 
list_print(list_t * list)
{
    if(list_len(list) < 1 || list == NULL)
        return;

    list_t * t = list;
    while(t != NULL)
    {
        printf("%s\n",t->value);
        t = t->next;
    }
}

bool 
list_destroy(list_t *list)
{
    if(list == NULL)
        return false;

    list_t * tmp = NULL;

    while(list != NULL)
    {
        tmp = list;
        list = list->next;
        free(tmp->value);
        free(tmp);
    }

    return true;

}

char * 
list_back(list_t *list)
{
    if(list_len(list) < 1)
        return "";

    list_t * t = list;
    while(t->next != NULL)
    {
        t = t->next;
    }

    return t->value;
}

char * 
list_front(list_t *list)
{
    if(list_len(list) < 1)
        return "";

    return list->value; 
}

bool 
list_popback(list_t *list)
{
    if(list_len(list) < 1)
        return false;

    if(list_len(list) == 1)
    {
        //list_destroy(list);
        //free(list);
        list->value = NULL;
        list = NULL;
        return true;
    }
    
    list_t * t = list;
    while(t->next->next != NULL)
    {
        t = t->next;
    }

    list_destroy(t->next);
    t->next = NULL;
    return true;
}

bool 
list_popfront(list_t **list)
{

    if(list_len(*list) < 1)
        return false;

    if(list_len(*list) == 1 || (*list)->next == NULL)
    {
        (*list)->value = NULL;
        list = NULL;
        return true;
    }

    list_t *tmp = (*list)->next;
    *list = NULL;
    *list = malloc(sizeof(list_t));
    *list = tmp;
    return true;
}

char * 
list_get(list_t *list, unsigned int index)
{
    if(index >= list_len(list)){
        printf("Index outta bounds.\n");
        return NULL;
    }

    unsigned 
    int pos = 0;
    list_t *tmp = list;

    while(tmp != NULL)
    {
        if(pos == index){
            break;
        }
        tmp = tmp->next;
        pos++;
    }

    return tmp->value;
}

