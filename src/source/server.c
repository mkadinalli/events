#include "../include/server.h"
#include "../include/pool.h"
#include "../include/lib.h"
#include "../include/map.h"
#include "../include/string_buffer.h"
#include "../include/requests.h"

int socketfd = 0;

tpool_t *thread_pool = NULL;

void signal_handler(int sig)
{
    if(sig == 20 || sig == 2)
        clean_up();
}

void clean_up()
{
    if (socketfd > 0)
    {
        close(socketfd);
    }
    if (thread_pool != NULL)
        tpool_destroy(thread_pool);

    printf("\n===Server is off rn===\n");
    exit(1);
}

void *handle_request(void *args)
{
    int *th = (int *)args;
    int their_socket = *th;

    char recv_buf[2];
    int bytes_received;
    string_t *b = string_create();
    string_t *json_b = string_create();

    char end_of_header[] = "\r\n\r\n";
    int marker = 0;

    FILE *ptr;
    bool file_reached = false;

    int error_code = OK;
    int req_method = 0;
    int file_type = 0;

    map_t * http_req = NULL;

    while ((bytes_received = recv(their_socket, recv_buf, 1, 0)))
    {
        if (bytes_received == -1)
        {
            perror("recv");
        }

        if (bytes_received == 0)
        {
            break;
        }

        if (!file_reached)
        {
            string_append(b, recv_buf[0]);
        }
        else
        {
            if(ptr && file_type == IMAGE)
                fwrite(recv_buf, 1, sizeof recv_buf - 1, ptr);

            if(file_type == JSON)
                string_append(json_b, recv_buf[0]);
        }

        if (recv_buf[0] == end_of_header[marker])
        {
            marker++;
        }
        else
        {
            marker = 0;
        }

        if (marker == 4)
        {
            file_reached = true;
            if((http_req = parse_http_req(b->chars)) != NULL)
            {
                if(!strcmp(map_get(http_req,"method"),"GET"))
                {
                    req_method = GET;
                    puts("method is get");
                    break;
                }else if (!strcmp(map_get(http_req,"method"),"POST"))
                {
                    req_method = POST;
                    puts("method is post");
                    if(!strcmp(map_get(http_req,"Content-Type"),"image/jpeg"))
                    {
                        file_type = IMAGE;
                        char filename[100];
                        sprintf(filename,"/home/vic/Desktop/ev2/events/files/image%lu.jpg",(unsigned long) time(NULL));

                       if((ptr = fopen(filename, "a")) == NULL){
                            puts("failed to open file");
                       }
                        puts("file is iamge");

                    }else if(!strcmp(map_get(http_req,"Content-Type"),"application/json"))
                    {
                        file_type = JSON;
                        puts("file is json");
                    }

                }else if (!strcmp(map_get(http_req,"method"),"PUT"))
                {
                    req_method = PUT;
                    puts("method is put");
                }

                else if (!strcmp(map_get(http_req,"method"),"PATCH"))
                {
                    req_method = PATCH;
                    puts("method is patch");
                }
                else if (!strcmp(map_get(http_req,"method"),"DELETE"))
                {
                    req_method = DELETE;
                    puts("method is delete");
                }
                else { error_code = BAD_REQ; puts("request is bad") ;}
            }
        }
        
        bzero(&recv_buf, sizeof recv_buf);
    }

    if(!file_reached) error_code = BAD_REQ;

    puts(b->chars);

    if(error_code == OK && req_method)
    {
        if(starts_with_word("/file",map_get(http_req,"url")))
        {
            //serve_file(); no json
        }
        else if(starts_with_word("/api",map_get(http_req,"url")))
        {
            //serve_api
            if(req_method == GET){
                //no_json
            }
            else {
                //json must
            }
        }
        else if(starts_with_word("/pages",map_get(http_req,"url")))
        {
            //serve_page no json
        }
        else{ 
            //serve error
        }
    }

    if(file_type == JSON) puts(json_b->chars);

    map_destroy(http_req);
    close(their_socket);

    return 0;
}

void accept_connections(int socketfd)
{
    struct sockaddr_storage their_address;
    socklen_t len = sizeof their_address;

    while (1)
    {
        int their_socket;
        if ((their_socket = accept(socketfd, (struct sockaddr *)&their_address, &len)) < 0)
        {
            perror("Accept");
            exit(EXIT_FAILURE);
        }

        tpool_add_work(thread_pool,handle_request,&their_socket);
        //handle_request(&their_socket);
    }
}

bool set_up_server(char *PORT)
{
    // PORT = port;
    struct addrinfo hints;
    struct addrinfo *server_info, *p;

    memset(&hints, 0, sizeof hints);

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    // 0 for success
    int status = getaddrinfo(NULL, PORT, &hints, &server_info);

    if (status != 0)
    {
        fprintf(stderr, "Error : %s", gai_strerror(status));
    }

    char address[INET6_ADDRSTRLEN];
    int port = 0;

    for (p = server_info; p != NULL; p = p->ai_next)
    {

        void *myaddr;
        int ip_version;

        int family = p->ai_family;

        if (family == AF_INET)
        {
            ip_version = IPV_4;
            struct sockaddr_in *myaddress = (struct sockaddr_in *)p->ai_addr;
            myaddr = &(myaddress->sin_addr);
            port = ntohs(myaddress->sin_port);
        }
        else if (family == AF_INET6)
        {
            ip_version = IPV_6;
            struct sockaddr_in6 *myaddress = (struct sockaddr_in6 *)p->ai_addr;
            myaddr = &(myaddress->sin6_addr);
            port = ntohs(myaddress->sin6_port);
        }

        if (ip_version)
        {
        }

        inet_ntop(p->ai_family, myaddr, address, INET6_ADDRSTRLEN);

        printf("Locust running on port :%d of this computer\n", port);

        socketfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);

        if (socketfd < 0)
        {
            perror("Socket : ");
        }
        else
            break;
    }

    if (p == NULL)
    {
        fprintf(stderr, "Error creating socket\n");
    }

    if ((status = bind(socketfd, p->ai_addr, p->ai_addrlen)) < 0)
    {
        perror("bind");
    }

    if ((status = listen(socketfd, BACKLOG)) < 0)
    {
        perror("listen");
    }

    thread_pool = tpool_create(2);

    accept_connections(socketfd);

    close(socketfd);
    free(server_info);

    return true;
}
