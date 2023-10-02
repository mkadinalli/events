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


#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<netinet/in.h>

char * get_ip_as_string(struct sockaddr *address)
{
  char *ip_string = malloc(INET6_ADDRSTRLEN);
  if(address->sa_family == AF_INET)
  {
    inet_ntop(AF_INET6,(struct in6_addr *) address,ip_string,INET6_ADDRSTRLEN);
    puts(ip_string);
    return ip_string;
  }

  inet_ntop(AF_INET,(struct in_addr *) address,ip_string,INET_ADDRSTRLEN);
  puts(ip_string);
  return ip_string;
}

int main(int argvc, char *args[])
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

    freeaddrinfo(res);
}

