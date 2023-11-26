#include "../../include/http/server.h"

#include "endpoints.h"
#include "web_sock.h"
#include "res_builder.h"

int socketfd = 0;

tpool_t *thread_pool = NULL;
conn_pool *cpool = NULL;
int server_fd = -1;
SSL_CTX *server_accept_ctx = NULL;
messge *message_queue;
fd_map_t *g_filedescriptor_map;



void signal_handler(int sig)
{
    if (sig == 20 || sig == 2)
        clean_up();
}

void clean_up()
{
    keep_chat_alive = false;
    fd_count_g = 1;
    cnd_signal(&poll_condition);

    if (server_fd > 0)
    {
        close(server_fd);
    }

    if (thread_pool != NULL)
        tpool_destroy(thread_pool);
    
    if(cpool != NULL)
        cpool_destroy(cpool);

    mtx_destroy(&(poll_mutex));
    cnd_destroy(&(poll_condition));
    exit(1);
}

int handle_request(void *args)
{
    int *ss = (int *)(args);
    int ssl = *ss;

    char recv_buf[1] = {0};
    char recv_buff_f[100] = {0};
    int bytes_received;
    string_t *b = string_create();
    string_t *json_b = string_create();

    char end_of_header[] = "\r\n\r\n";
    int marker = 0;
    bool file_reached = false;

    int error_code = OK;
    int req_method = 0;
    int file_type = 0;

    map_t *http_req = NULL;

    int lopps = 0;

    long content_len = -1;
    long total_received = 0;

    while (true)
    {

        bytes_received = recv(ssl, file_reached ? recv_buff_f : recv_buf, file_reached ? 99 : 1, 0);

        if (bytes_received == -1)
        {
            perror("recving");
            ERR_print_errors_fp(stderr);
            break;
        }

        if (!file_reached)
            string_append(b, recv_buf[0]);
        else
        {
            if (file_type == JSON)
                string_concat(json_b, recv_buff_f, bytes_received);
            else
                break;
        }

        if (recv_buf[0] == end_of_header[marker])
            marker++;
        else
            marker = 0;

        if (file_reached)
            total_received += bytes_received;

        if ((bytes_received <= 0 && file_reached) || total_received == content_len)
        {
            break;
        }

        if (marker == 4)
        {
            file_reached = true;
            lopps = 0;

            if ((http_req = parse_http_req(b->chars)) == NULL)
            {
                // write_BAD(ssl);
                break;
            }

            map_print(http_req);

            if (map_get_ref(http_req, "content-type") != NULL)
            {

                if (starts_with_word("application/json", map_get_ref(http_req, "content-type")) ||
                    starts_with_word("text/plain", map_get_ref(http_req, "content-type")))
                {

                    file_type = JSON;
                }
                else
                {
                    if (map_get_ref(http_req, "content-length") == NULL)
                    {
                        error_code = BAD_REQ;
                        write_BAD(ssl);
                        break;
                    }

                    content_len = strtol(map_get_ref(http_req, "content-length"), NULL, 0);
                    break;
                }
            }

            if (starts_with_word(map_get_ref(http_req, "method"), "GET"))
            {
                req_method = GET;
                break;
            }

            if (starts_with_word(map_get_ref(http_req, "method"), "POST"))
            {
                req_method = POST;
            }

            if (starts_with_word(map_get_ref(http_req, "method"), "PUT"))
            {
                req_method = PUT;
            }

            if (starts_with_word(map_get_ref(http_req, "method"), "DELETE"))
            {
                req_method = DELETE;
                break;
            }

            if (starts_with_word(map_get_ref(http_req, "method"), "OPTIONS"))
            {
                req_method = OPTIONS;
                break;
            }

            if (req_method == 0)
            {
                error_code = BAD_REQ;
                puts("===Method not found=====");
                break;
            }

            if (map_get_ref(http_req, "content-length") == NULL && (req_method == POST || req_method == PUT))
            {
                write_BAD(ssl);
                break;
            }

            content_len = strtol(map_get_ref(http_req, "content-length"), NULL, 0);

            printf("Content len = %ld\n", content_len);
        }

        file_reached ? bzero(&recv_buff_f, sizeof recv_buff_f)
                     : bzero(&recv_buf, sizeof recv_buf);

        if ((lopps >= 4096 && !file_reached) || (lopps > (1024 * 1024 * 20)))
            break;

        lopps++;
    }

    if (!file_reached)
        error_code = BAD_REQ;

    if (error_code != OK && req_method == 0)
    {
        write_BAD(ssl);
        goto clean_me;
    }

    char file_dir[200] = "./files";
    char *f = strcat(file_dir, map_get_ref(http_req, "url"));
    puts(f);

    switch (req_method)
    {
    case GET:
            if (starts_with_word("/websock", map_get(http_req,"url")))
            {
                method_get_ws(ssl,http_req);
            }else{

                method_get(ssl, map_get_ref(http_req, "url"));
            }
        break;
    case POST:
        if(file_type == JSON){
            method_post(ssl, map_get_ref(http_req, "url"), string_create_copy(json_b->chars));
        }else{
            method_post_file(ssl,map_get_ref(http_req, "url"),map_get_ref(http_req,"content-type"),content_len);
        }
        // write_OK(ssl,"");
        break;

    case PUT:
        method_put(ssl, map_get_ref(http_req, "url"), string_create_copy(json_b->chars));
        break;

    case DELETE:
        method_delete(ssl, map_get_ref(http_req, "url"));
        break;

    case OPTIONS:
        write_OK(ssl, "");
        break;

    default:
        write_BAD(ssl);
        break;
    }

clean_me:

    b = NULL;
    map_destroy(http_req);
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
            // perror("Accept");
            continue;
        }

        tpool_add_work(thread_pool, handle_request, &their_socket);
    }
}

bool set_up_server(char *PORT)
{
    signal(20,signal_handler);
    signal(2,signal_handler);
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
    int yes = 1;

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

        printf("API running on port :%d of this computer\n", port);

        server_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);

        if (server_fd < 0)
        {
            perror("Socket : ");
        }
        else
        {
            setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes);
            break;
        }
    }

    if (p == NULL)
    {
        fprintf(stderr, "Error creating socket\n");
    }

    if ((status = bind(server_fd, p->ai_addr, p->ai_addrlen)) < 0)
    {
        perror("bind");
        exit(1);
    }

    if ((status = listen(server_fd, BACKLOG)) < 0)
    {
        perror("listen");
    }

    thread_pool = tpool_create(5);

    tpool_add_work(thread_pool, startChartSystem, NULL);
    tpool_add_work(thread_pool,start_queue,&message_queue);

    cpool = create_conn_pool(5);

    g_filedescriptor_map = fd_map_create();
    message_queue = message_create();

    accept_connections(server_fd);

    close(server_fd);
    free(server_info);

    return true;
}