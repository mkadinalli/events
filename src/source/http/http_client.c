#include "http_client.h"

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

bool http_client_connect()
{
    int status,sock;
    struct addrinfo hints;
    struct addrinfo * res,*p;

    memset(&hints,0,sizeof hints);

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    status = getaddrinfo(NULL,"2000",&hints,&res);

    if(status != 0){
      fprintf(stderr,"%s\n",gai_strerror(status));
      exit(1);
    }

    for(p = res; p ; p = p->ai_next)
    {
      get_ip_as_string(p->ai_addr);

      sock = socket(p->ai_family,p->ai_socktype,p->ai_protocol);

      if(sock < 0) break;

       if(connect(sock,p->ai_addr,p->ai_addrlen))
       {
        fprintf(stderr,"failed to connect\n");
       }else{ break; }
    }

    if(p == NULL){
        fprintf(stderr,"failed to create socket\n");
    }

    char rec_buff[1024] = {0};

    status = recv(sock,rec_buff,1024,0);

    if(status < 1)
    {
      puts("an error");
    }

    puts(rec_buff);

    freeaddrinfo(res);
}

http_client *http_client_create()
{
  http_client * client = malloc(sizeof(http_client));

  client->headers = map_create();

  client->url = NULL;

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