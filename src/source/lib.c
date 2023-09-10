#include "../include/lib.h"

#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<stdbool.h>

char *string_removechar(char c, char *buffer, size_t len)
{
    string_t * temp = string_create();
    for(size_t i = 0 ; i < len; i++)
    {
        if(buffer[i] == c)
        {
            continue;
        }

        string_append(temp,buffer[i]);
    }
    string_append(temp,'\0');
    char *chars = temp->chars;
    free(temp);
    return chars;
}

char *string_replacechar(char previous, char current, char *buffer, size_t len){
    string_t * temp = string_create();
    for(size_t i = 0 ; i < len; i++)
    {
        if(buffer[i] == previous)
        {
            string_append(temp,current);
            continue;
        }

        string_append(temp,buffer[i]);
    }
    string_append(temp,'\0');
    char *chars = temp->chars;
    free(temp);
    return chars;
}

char *string_removechar_at(unsigned int pos, char *buffer, size_t len){
    string_t * temp = string_create();
    for(size_t i = 0 ; i < len; i++)
    {
        if(i == pos)
        {
            continue;
        }

        string_append(temp,buffer[i]);
    }
    string_append(temp,'\0');
    char *chars = temp->chars;
    free(temp);
    return chars;
}

int string_last_index_of(char c, char *str, unsigned int len){
    int pos = -1;
    int i;
    for(i = len-1;i > -1; i--)
    {
        if(str[i] == c){
            pos = i;
            break;
        };
    }
    return pos;
}


char *string_sub(unsigned int start, unsigned int stop, char *buffer, unsigned int len){
    string_t * temp = string_create();
    for(unsigned int i = 0 ; i < len; i++)
    {
        if(i >= start && i <= stop)
        {
            string_append(temp,buffer[i]);
        }

        continue;
    }
    string_append(temp,'\0');
    char *chars = temp->chars;
    free(temp);
    return chars;
}

bool endswith(char c, char *str){ 
    if(str[strlen(str)-1] == c) return true;
    return false;
}

list_t *split(char delim, char *str, size_t str_size){
    list_t * myvec;
    string_t * bld;

    memset(&myvec,0,sizeof myvec);
    memset(&bld,0,sizeof bld);

    myvec = list_create();
    bld = string_create();

 
    for(size_t i =0; i < str_size;i++){
        if(str[i] == delim)
        {
            string_append(bld,'\0');
            list_pushback(myvec,bld->chars);

            string_destroy(bld);
            bld = string_create();
            
            continue;
        }


        string_append(bld,str[i]);
    }
    string_append(bld,'\0');
    list_pushback(myvec,bld->chars);

    return myvec;
}

list_t *split_lim(char delim, char *str, size_t str_size, int lim){
    list_t * myvec;
    string_t * bld;

    memset(&myvec,0,sizeof myvec);
    memset(&bld,0,sizeof bld);

    myvec = list_create();
    bld = string_create();

    bool found = false;
    int times_found = 1;

    for(size_t i =0; i < str_size;i++){

        if((str[i] == delim && !found) && times_found < lim)
        {
            string_append(bld,'\0');
            list_pushback(myvec,bld->chars);
            string_destroy(bld);
            bld = string_create();
            times_found++;
            if(times_found == lim) found = true;
            continue;
        }

        string_append(bld,str[i]);
    }

    string_append(bld,'\0');
    list_pushback(myvec,bld->chars);
    return myvec;
}
//char *terminate_string(char *str, size_t len){}