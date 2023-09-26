#include "../../include/http/server.h"

int socketfd = 0;

tpool_t *thread_pool = NULL;

void signal_handler(int sig)
{
    if (sig == 20 || sig == 2)
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

    char recv_buf[1] = {0};
    char recv_buff_f[100] = {0};
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

    map_t *http_req = NULL;

    int lopps = 0;

    while (true)
    {
        bytes_received = recv(their_socket, file_reached ? recv_buff_f : recv_buf, file_reached ? 99 : 1, 0);

        if (bytes_received == -1)
        {
            perror("recv");
            break;
        }

        if (!file_reached)
        {
            string_append(b, recv_buf[0]);
            // puts("appending to header------->>>");
        }
        else
        {
            if (ptr && file_type == IMAGE)
                fwrite(recv_buff_f, 1, bytes_received, ptr);

            if (file_type == JSON)
            {
                string_concat(json_b, recv_buff_f, bytes_received);
                // printf("Concat to json ==== %s\n",recv_buff_f);
            }
        }

        if (recv_buf[0] == end_of_header[marker])
        {
            marker++;
        }
        else
        {
            marker = 0;
        }

        if (bytes_received < 99 && file_reached)
        {
            printf("Breaking after %d\n loops", lopps);
            break;
        }

        if (marker == 4)
        {

            // recv_buf

            file_reached = true;
            lopps = 0;

            if ((http_req = parse_http_req(b->chars)) == NULL)
            {
                write_BAD(their_socket);

                return NULL;
            }

            if (!strcmp(map_get(http_req, "method"), "GET"))
            {
                req_method = GET;
                puts("method is get");
                break;
            }else{

                if (!strcmp(map_get(http_req, "Content-Type"), "image/jpeg"))
                {
                    file_type = IMAGE;
                    char filename[100];
                    sprintf(filename, "/home/vic/Desktop/ev2/events/files/image%lu.jpg", (unsigned long)time(NULL));

                    if ((ptr = fopen(filename, "a")) == NULL)
                    {
                        puts("failed to open file");
                    }
                    puts("file is image");
                    // break;
                }
            else if (!strcmp(map_get(http_req, "Content-Type"), "application/json"))
            {
                    file_type = JSON;
                    puts("file is json");
                    // break;
            }

            if (!strcmp(map_get(http_req, "method"), "POST"))
            {
                req_method = POST;
                puts("method is post");

            }
            else if (!strcmp(map_get(http_req, "method"), "PUT"))
            {
                req_method = PUT;
                puts("method is put");
                // break;
            }

            else if (!strcmp(map_get(http_req, "method"), "PATCH"))
            {
                req_method = PATCH;
                puts("method is patch");
                // break;
            }
            else if (!strcmp(map_get(http_req, "method"), "DELETE"))
            {
                req_method = DELETE;
                puts("method is delete");
                // break;
            }
            else
            {
                error_code = BAD_REQ;
                puts("request is bad");
                break;
            }
            }
        }

        file_reached ? bzero(&recv_buff_f, sizeof recv_buff_f)
                     : bzero(&recv_buf, sizeof recv_buf);

        lopps++;
    }

    if (!file_reached)
        error_code = BAD_REQ;

    if (error_code != OK && req_method == 0)
    {
        write_BAD(their_socket);
        goto clean_me;
    }

    char file_dir[200] = "/home/vic/Desktop/ev2/events";
    char *f = strcat(file_dir, map_get(http_req, "url"));
    puts(f);

    if (starts_with_word("/files", map_get(http_req, "url")))
    {
        upload_file(f, "image/jpeg", their_socket);
    }
    else if (starts_with_word("/api", map_get(http_req, "url")))
    {
        // serve_api
        switch (req_method)
        {
        case GET:
            serve_JSON(their_socket, map_get(http_req, "url"));
            break;
        default:
            receive_json(their_socket,
                         map_get(http_req, "url"),
                         string_create_copy(json_b->chars));
            break;
        case 0:
            write_BAD(their_socket);
            break;
        }
    }
    else if (starts_with_word("/js", map_get(http_req, "url")))
    {
        upload_file(f, "text/javascript", their_socket);
    }
    else if (starts_with_word("/css", map_get(http_req, "url")))
    {
        upload_file(f, "text/css", their_socket);
    }
    else if (starts_with_word("/pages", map_get(http_req, "url")))
    {
        upload_file(f, "text/html", their_socket);
    }
    else
    {
        write_404(their_socket);
        puts("404 not found");
    }

    if (file_type == JSON)
    {
        puts(json_b->chars);
        // puts("json printed");
    }

clean_me:

    b = NULL;
    map_destroy(http_req);
    close(their_socket);
    puts("closed socket");
    return NULL;
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

        tpool_add_work(thread_pool, handle_request, &their_socket);
        // handle_request(&their_socket);
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
