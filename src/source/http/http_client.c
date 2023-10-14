#include "http_client.h"
#include <stdarg.h>

char *get_ip_as_string(struct sockaddr *address)
{
  char *ip_string = malloc(INET6_ADDRSTRLEN);
  if (address->sa_family == AF_INET)
  {
    inet_ntop(AF_INET6, (struct in6_addr *)address, ip_string, INET6_ADDRSTRLEN);
    return ip_string;
  }

  inet_ntop(AF_INET, (struct in_addr *)address, ip_string, INET_ADDRSTRLEN);
  return ip_string;
}

int http_client_create_socket(char *address_, char *port,struct sockaddr **host)
{
  int status, sock;
  struct addrinfo hints;
  struct addrinfo *res, *p;

  memset(&hints, 0, sizeof hints);

  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_CANONNAME | AF_INET;

  status = getaddrinfo(address_, port, &hints, &res);

  if (status != 0)
  {
    fprintf(stderr, "%s\n", gai_strerror(status));
    return -1;
  }

  for (p = res; p; p = p->ai_next)
  {
    get_ip_as_string(p->ai_addr);

    sock = socket(p->ai_family, p->ai_socktype, p->ai_protocol);

    if (sock < 0)
      continue;

    if (!connect(sock, p->ai_addr, p->ai_addrlen))
    {
      *host = malloc(sizeof p->ai_addr);
      memcpy(*host,p->ai_addr,p->ai_addrlen);
      break;
    }

    perror("connect");
  }

  if (p == NULL)
  {
    
    puts("NULL");
    return -1;
  }

  freeaddrinfo(res);
  return sock;
}

SSL *http_client_create_ssl(char *address_, SSL_CTX *ctx, int sock)
{
  SSL *ssl = NULL;
  (void)SSL_library_init();
  SSL_load_error_strings();
  // OPENSSL_config(NULL);

#if defined(OPENSSL_THREADS)
// to do
#endif

  int res;

  ssl = SSL_new(ctx);
  if (ssl == NULL)
  {
    puts("Failed to create ssl");
    return NULL;
  }

  const long flags = SSL_OP_NO_SSLv2 | SSL_OP_NO_SSLv3 | SSL_OP_NO_COMPRESSION;
  SSL_CTX_set_options(ctx, flags);

  res = SSL_set_tlsext_host_name(ssl, address_);
  if (res != 1)
  {
    puts("Failed to set host name");
    return NULL;
  }

  const char *preffered_ciphers = "HIGH:!aNULL:!kRSA:!PSK:!SRP:!MD5:!RC4";

  res = SSL_set_cipher_list(ssl, preffered_ciphers);

  if (res != 1)
  {
    puts("Failed to set cipher list");
    return NULL;
  }

  res = SSL_set_fd(ssl, sock);
  if (res != 1)
  {
    puts("Failed to set fd");
    return NULL;
  }
//////////////////////////////////////////////
  //BIO_set_nbio(SSL_get_rbio(ssl),1);
  //BIO_set_nbio(SSL_get_wbio(ssl),1);

///////////////////////////////////////////
  puts("about to connect");

  if(ssl == NULL)
  {
    puts("ssl is nulllllll");
    exit(1);
  }

  res = SSL_connect(ssl);
  if (res != 1)
  {
    puts("Failed to connect");
    return NULL;
  }

  puts("connected ....");

  res = SSL_do_handshake(ssl);
  if (res != 1)
  {
    puts("Failed to handshake");
    return NULL;
  }

  puts("Handshake");

  return ssl;
}

http_client *http_client_create()
{
  http_client *client = malloc(sizeof(http_client));

  client->headers = NULL;

  client->url = NULL;

  client->http_version = malloc(10 * sizeof(char));

  client->port = NULL;

  client->address = NULL;

  strcpy(client->http_version, "HTTP/1.1");

  client->file_size = 0;

  return client;
}

bool http_client_set_url(char *url, http_client *client)
{
  if (!url || !client)
    return false;

  int len = strlen(url);

  client->url = malloc(len + 1);
  if (!client->url)
    return false;

  strcpy(client->url, url);

  return true;
}

bool http_client_set_method(methods m, http_client *client)
{
  switch (m)
  {
  case GET:
    return http_client_set_method_str("GET", client);
    break;
  case POST:
    return http_client_set_method_str("POST", client);
    break;
  default:
    return http_client_set_method_str("GET", client);
  }

  return false;
}

bool http_client_set_method_str(char *m, http_client *client)
{
  if (!client || !m)
    return false;
  int l = strlen(m);
  client->method = malloc(l + 1);
  strcpy(client->method, m);
  return true;
}

bool http_client_set_address(char *address, http_client *client)
{
  if (!client || !address)
    return false;
  int l = strlen(address);
  client->address = malloc(l + 1);
  strcpy(client->address, address);
  return true;
}

bool http_client_set_port(char *port, http_client *client)
{
  if (!client || !port)
    return false;
  int l = strlen(port);
  client->port = malloc(l + 1);
  strcpy(client->port, port);
  return true;
}

bool http_client_set_header(char *key, char *value, http_client *client)
{
  if (!key || !value || !client)
    return false;

  if (client->headers == NULL)
    client->headers = map_create();

  return map_add(client->headers, key, value);
}

bool http_client_append_file(char *path, http_client *client)
{
  if (!client || client->body)
    return false;

  FILE *fptr;
  size_t file_size;

  if (!(fptr = fopen(path, "rb")))
    return false;

  fseek(fptr, 0, SEEK_END);

  file_size = ftell(fptr);
  client->file_size = file_size;

  client->body = malloc(file_size + 1);
  bzero(client->body, file_size + 1);

  rewind(fptr);
  size_t read_bytes = 0;
  bool out = true;
  size_t offset = 0;

  while ((read_bytes = fread(client->body + offset, 1, 100, fptr)))
  {
    if (read_bytes != 100)
    {
      if (ferror(fptr))
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

bool http_client_append_string(char *str, http_client *client)
{

  puts("=============================");

  puts(str);

  puts("=============================");

  if (!str || !client || client->body)
    return false;

  int len = strlen(str);

  client->body = malloc(len + 1);

  if (!client->body)
    return false;

  client->file_size = len;

  strcpy(client->body, str);

  puts("=============================");

  puts(client->body);

  puts("=============================");

  return true;
}

bool http_client_connect(http_client *client)
{
  if (!client->url || !client->method || !client || !client->address)
  {
    puts("missing members");
    return false;
  }


  SSL_CTX *ctx = NULL;
  (void)SSL_library_init();
  SSL_load_error_strings();

  const SSL_METHOD *method = SSLv23_method();

  if (method == NULL)
  {
    // handle error
    puts("Method failed");
    return false;
  }

  ctx = SSL_CTX_new(method);

  if (ctx == NULL)
  {
    puts("failed to start context");
    return false;
  }

  int sock;
  struct sockaddr *remote_host = NULL;

  if ((sock = http_client_create_socket(client->address, client->port,&remote_host)) == -1)
  {
    puts("failed to create socket");
    return false;
  }


  if(remote_host == NULL)
    puts("Host is null");
  else
  {
    //http_client_set_host(remote_host,client);
    //puts(remote_host->sa_data);
  }

  SSL *ssl = NULL;
  char *header = http_client_write_header(client);

  puts(header);

  if ((ssl = http_client_create_ssl(client->address, ctx, sock)) == NULL)
  {
    puts("failed to create ssl");
    return false;
  }

  //puts("<---ssl created--->");
  //puts(header);

  if ((SSL_write(ssl, header, strlen(header))) == -1)
  {
    puts("Error sending");
    return false;
  }

  bool out = true;

  if (client->body)
  {
    int size;
    int offset = 0;
    int rem = 100;
    size_t total_sent = 0;

    while (true)
    {
      int b_sent;

      if (client->file_size <= 100)
      {
        size = client->file_size;
      }else{ size = rem; }

      b_sent = SSL_write(ssl, client->body + offset, size);

      total_sent += b_sent;

      if(b_sent < 1)
      {
        if(b_sent == -1) out = false;
        break;
      }

      if(total_sent >= client->file_size)
        break;

      if (client->file_size <= 100)
        break;

      rem = client->file_size - total_sent;

      if (rem < 100)
      {
        size = rem;
      }
      else
      {
        size = 100;
      }

      offset += 100;

      printf("%d ",b_sent);
    }

    puts("wrote body");
  }

  //puts("**********************************");
  out = http_client_receive_response(ssl, client);

  SSL_free(ssl);
  SSL_CTX_free(ctx);
  close(sock);
  return out;
}

bool http_client_receive_response(SSL *sock, http_client *client)
{
  char recv_buf[1] = {0}, recv_buff_f[100] = {0}, end_of_header[] = "\r\n\r\n";
  int bytes_received, file_type = JSON, lopps = 0, marker = 0;
  string_t *b = string_create(), *json_b = string_create();

  bool file_reached = false;

  map_t *http_req = NULL;

  bool out = true;

  struct pollfd pfds[1];

  pfds[0].fd = SSL_get_fd(sock);
  pfds[0].events = POLLIN;


  while (true)
  {
    int num_evs = poll(pfds,1,10000);
    

    if(num_evs < 1)
    {
      if(num_evs == 0)
      {
        puts(" ======= Connection Time out ========");
      }else{
          puts("<<<<<<< Connection failed >>>>>>>>>");
      }
      out = false;
      continue;
    }
    else
    {
      int pollin_happened = pfds[0].revents & POLLIN;

      if(!pollin_happened)
      {
         break; }
    }

    bytes_received = SSL_read(sock, file_reached ? recv_buff_f : recv_buf, file_reached ? 100 : 1);
    if (bytes_received == -1)
    {
      perror("recv");
      out = false;
      break;
    }

    if (!file_reached)
      string_append(b, recv_buf[0]);
    else
    {
      if (file_type == JSON)
        string_concat(json_b, recv_buff_f, bytes_received);
    }

    if (recv_buf[0] == end_of_header[marker])
      marker++;
    else
      marker = 0;

    if (bytes_received <= 0 /* && file_reached*/)
    {
      break;
    }

    if (marker == 4)
    {
      file_reached = true;
      lopps = 0;

      if ((http_req = parse_http_response(b->chars)) == NULL)
        return false;

      //map_print(http_req);

      if (!map_get(http_req, "content-type"))
        return false;

      if (!strcmp(map_get(http_req, "content-type"), "text/html"))
        file_type = JSON;
    }

    file_reached ? bzero(&recv_buff_f, sizeof recv_buff_f)
                 : bzero(&recv_buf, sizeof recv_buf);

    lopps++;
  }

  if (file_type == JSON)
  {
    client->response = json_b->chars;
  }

  return out;
}

void dbg_client(http_client *ct)
{
  puts(ct->address);
  puts(ct->body);
  puts(ct->http_version);
  puts(ct->method);
  puts(ct->port);
}

char *http_client_write_header(http_client *ct)
{
  string_t *head = string_create();
  char onst[500];

  sprintf(onst, "%s %s %s\r\n", ct->method, ct->url, ct->http_version);

  string_concat(head, onst, strlen(onst));

  if (ct->headers == NULL)
  {
    char *chd = head->chars;
    puts("No headers");
    free(head);
    return chd;
  }

  map_t *tmp = ct->headers;

  while (tmp)
  {
    string_concat(head, tmp->key, strlen(tmp->key));
    string_append(head, ':');
    string_append(head, ' ');
    string_concat(head, tmp->value, strlen(tmp->value));
    string_concat(head, "\r\n", 2);

    tmp = tmp->next;
  }

  string_concat(head, "\r\n", 2);

  char *chd = head->chars;
  free(head);
  return chd;
}

bool http_client_set_host(struct sockaddr * host,http_client *client)
{
  char host_name[1024];
  char service[50];
  int status;

  if((status = getnameinfo(host,sizeof(struct sockaddr_in),host_name,sizeof host_name,service,sizeof service,0)) != 0)
  {
    puts(gai_strerror(status));
    return false;
  }

  //int port = http_client_get_service_port(service);

  //char host_port[1074];

  //sprintf(host_port,"%s:%d",host_name,port);
  
  return http_client_set_header("Host","sandbox.safaricom.co.ke",client);
}

int http_client_get_service_port(char *service_name)
{
  printf("Service name is -> %s\n",service_name);
  struct servent *sv = NULL;

  char proto[4] = "tcp";

  sv =  getservbyname(service_name,proto);

  if(sv == NULL) return -1;
  //getservbyname(service_name,"TCP");
  return ntohs(sv->s_port);
}


void http_client_destroy(http_client *client)
{
  if(client == NULL) return;

  if(client->address) free(client->address);
  if(client->body) free(client->body);
  if(client->response) free(client->response);
  if(client->url) free(client->url);
  if(client->headers) map_destroy(client->headers);
  if(client->port) free(client->port);
  if(client->method) free(client->method);
  if(client->http_version) free(client->http_version);

  free(client);
}