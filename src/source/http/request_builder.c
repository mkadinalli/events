#include "res_builder.h"

char *response_builder_to_string(response_builder *res){
    if(!res || !res->status_code || !res->status_name) {
        puts("Error");
        return NULL;
    
    }

    int b_len = 0;

    if(res->body){ 
        b_len = strlen(res->body);
        char cl_str[10];
        sprintf(cl_str,"%d", b_len);
        response_builder_set_header(res,"Content-Length",cl_str);
    }

    char firstLine[200];
    sprintf(firstLine,"%s %s %s\r\n",res->http_version,res->status_code,res->status_name);

    string_t *response = string_create_from_string(firstLine);

    map_t * htmp = res->headers;

    while(htmp){
        char line[200];
        sprintf(line,"%s: %s\r\n",htmp->key,htmp->value);
        string_concat(response,line,strlen(line));
        htmp = htmp->next;
    }

    string_concat(response,"\r\n",2);

    if(res->body){  
        string_concat(response,res->body,b_len);
    }

    char *r_ch = response->chars;

    free(response);

    return r_ch;
}

bool response_builder_set_header(response_builder *res,char *key,char *value){
    if(res){
        if(!res->headers) {
            res->headers = map_create();
        }
            map_add(res->headers,key,value);
        return true;
    }
    return false;
}

bool response_builder_set_code(response_builder *res, char*code){
    if(res && code){
        int len = strlen(code);

        res->status_code = malloc(len+1);
        strcpy(res->status_code,code);
        return true;
    }

    return false;
}

bool response_builder_set_status_name(response_builder *res, char *status){
    if(res && status){
        int len = strlen(status);

        res->status_name = malloc(len+1);
        strcpy(res->status_name,status);
        return true;
    }

    return false;
}

bool response_builder_set_body(response_builder *res, char *body){
        if(res && body){
        int len = strlen(body);

        res->body = malloc(len+1);
        strcpy(res->body,body);
        return true;
    }

    return false;
}

void response_builder_free(response_builder *res){
    if(!res) return;
    if(res->body) free(res->body);
    if(res->headers) map_destroy(res->headers);
    if(res->http_version) free(res->http_version);
    if(res->status_code) free(res->status_code);
    if(res->status_name) free(res->status_name);
    free(res);
    res = NULL;
}

response_builder * response_builder_create(){
    response_builder * r = malloc(sizeof(response_builder));
    memset(r,0,sizeof(response_builder));
    r->http_version = malloc(sizeof("HTTP/1.1") +1);
    strcpy(r->http_version,"HTTP/1.1");

    //r->headers = map_create();

    return r;
}