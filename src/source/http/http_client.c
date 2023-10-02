#include "http_client.h"
#include <stdarg.h>

char * get_ip_as_string(struct sockaddr *address)
{
  char *ip_string = malloc(INET6_ADDRSTRLEN);
  if(address->sa_family == AF_INET)
  {
    inet_ntop(AF_INET6,(struct in6_addr *) address,ip_string,INET6_ADDRSTRLEN);
    return ip_string;
  }

  inet_ntop(AF_INET,(struct in_addr *) address,ip_string,INET_ADDRSTRLEN);
  return ip_string;
}

int http_client_create_socket(char *address_,...)
{
    int status,sock;
    struct addrinfo hints;
    struct addrinfo * res,*p;

    va_list args;

    va_start(args,address_);

    memset(&hints,0,sizeof hints);

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    status = getaddrinfo(va_arg(args,char *),va_arg(args,char *),&hints,&res);

    va_end(args);

    if(status != 0){
      fprintf(stderr,"%s\n",gai_strerror(status));
      return -1;
    }

    for(p = res; p ; p = p->ai_next)
    {
      get_ip_as_string(p->ai_addr);

      sock = socket(p->ai_family,p->ai_socktype,p->ai_protocol);

      if(sock < 0) break;

       if(!connect(sock,p->ai_addr,p->ai_addrlen))
       { break; }
    }

    if(p == NULL){
        return -1;
    }

    freeaddrinfo(res);

    return sock;
}

http_client *http_client_create()
{
  http_client * client = malloc(sizeof(http_client));

  client->headers = NULL;

  client->url = NULL;

  client->http_version = malloc(10 * sizeof(char));

  client->port = NULL;

  client->address = NULL;

  strcpy(client->http_version,"HTTP/1.1");

  client->file_size = 0;

  return client;

}

bool http_client_set_url(char *url,http_client *client)
{
  if(!url || !client) return false;

  int len = strlen(url);

  client->url = malloc(len+1);
  if(!client->url) return false;

  strcpy(client->url,url);

  return true;
}

bool http_client_set_method(methods m,http_client *client)
{
  switch(m)
  {
    case GET:
      return http_client_set_method_str("GET",client);
    break;
    case POST:
      return http_client_set_method_str("POST",client);
    break;
    default:
      return http_client_set_method_str("GET",client);
  }

  return false;
}

bool http_client_set_method_str(char *m,http_client *client)
{
  if(!client || !m) return false;
  int l = strlen(m);
  client->method = malloc(l+1);
  strcpy(client->method,m);
  return true;
}

bool http_client_set_address(char *address,http_client *client)
{
  if(!client || !address) return false;
  int l = strlen(address);
  client->address = malloc(l+1);
  strcpy(client->address,address);
  return true;
}

bool http_client_set_port(char *port,http_client *client)
{
  if(!client || !port) return false;
  int l = strlen(port);
  client->port = malloc(l+1);
  strcpy(client->port,port);
  return true;
}

bool http_client_set_header(char *key,char *value,http_client *client)
{
  if(!key || !value || !client) return false;

  return map_add(client->headers,key,value);
}

bool http_client_append_file(char *path,http_client *client)
{
  if(!client || client->body) return false;

  FILE *fptr;
  size_t file_size;

  if(!(fptr = fopen(path,"rb")))
    return false;
  
  fseek(fptr,0,SEEK_END);

  file_size = ftell(fptr);
  client->file_size = file_size;
  
  client->body = malloc(file_size+1);
  bzero(client->body,file_size+1);

  rewind(fptr);
  size_t read_bytes = 0;
  bool out = true;
  size_t offset = 0;

  while((read_bytes = fread(client->body+offset,1,100,fptr)))
  {
    if(read_bytes != 100)
    {
      if(ferror(fptr))
      {
        free(client->body);
        out = false;
        break;
      }
    }
    offset += 100;
  }

  return out;
}


bool http_client_append_string(char *str,http_client *client)
{
  if(!str || !client || client->body) return false;

  int len = strlen(str);

  client->body = malloc(len+1);

  if(!client->body) return false;

  strcpy(client->body,str);

  return true;
}

bool http_client_connect(http_client * client)
{
  if(!client->url || !client->method || !client || !client->address)
   { puts("missing members"); return false;}

  int sock;
  
  if((sock = http_client_create_socket(client->address,client->port)) == -1)
   {
    puts("failed to create socket"); return false;}

  return true;
}

void dbg_client(http_client *ct)
{
  puts(ct->address);
  puts(ct->body);
  puts(ct->http_version);
  puts(ct->method);
  puts(ct->port);
}