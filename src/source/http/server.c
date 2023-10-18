#include "../../include/http/server.h"
#include <openssl/ssl.h>
#include <openssl/err.h>

int socketfd = 0;

tpool_t *thread_pool = NULL;
conn_pool *cpool = NULL;

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

    puts("\n===Server is off rn===\n");
    exit(1);
}

int handle_request(void *ss)
{
    puts("=================================================hhh");

    ssl_holder *sh = ss;

    SSL *ssl = *(sh->ssl_strct);

    puts("freeing");

    //SSL_free(ssl);

    puts("freed");



    char recv_buf[1] = {0};
    char recv_buff_f[100] = {0};
    int bytes_received;
    puts("rrrrrrrrrrrr");
    string_t *b = string_create();
    string_t *json_b = string_create();

    char end_of_header[] = "\r\n\r\n";
    int marker = 0;


    FILE *ptr = NULL;
    bool file_reached = false;

    int error_code = OK;
    int req_method = 0;
    int file_type = 0;

    map_t *http_req = NULL;

    int lopps = 0;

    char filename[200] = {0};

    while (true)
    {

        bytes_received = SSL_read(ssl, file_reached ? recv_buff_f : recv_buf, file_reached ? 99 : 1);

        
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
            if (ptr && file_type == IMAGE)
                fwrite(recv_buff_f, 1, bytes_received, ptr);

            if (file_type == JSON)
            {
                    string_concat(json_b, recv_buff_f, bytes_received);
                   // puts(json_b);
            }

            puts(recv_buff_f);
                
        }

        if (recv_buf[0] == end_of_header[marker])
            marker++;
        else
            marker = 0;

        if (bytes_received <= 0 && file_reached)
            break;

        if (marker == 4)
        {
            file_reached = true;
            lopps = 0;

            if ((http_req = parse_http_req(b->chars)) == NULL)
            {
                write_BAD(ssl);

                return 1;
            }

            //map_print(http_req);

            if (map_get(http_req, "Content-Type") != NULL)
            {
                if (starts_with_word("image/jpeg",map_get(http_req, "Content-Type")))
                {
                    file_type = IMAGE;
                    sprintf(filename, "/home/vic/Desktop/ev2/events/files/image%lu.jpg", (unsigned long)time(NULL));
                    ptr = fopen(filename, "a");
                }
                else if (starts_with_word("application/json",map_get(http_req, "Content-Type")))
                    file_type = JSON;
            }

            if (!strcmp(map_get(http_req, "method"), "GET"))
            {
                req_method = GET;
                break;
            }
            else if (!strcmp(map_get(http_req, "method"), "POST"))
                req_method = POST;
            else if (!strcmp(map_get(http_req, "method"), "PUT"))
                req_method = PUT;

            else if (!strcmp(map_get(http_req, "method"), "PATCH"))
                req_method = PATCH;
            else if (!strcmp(map_get(http_req, "method"), "DELETE"))
            {
                req_method = DELETE;
                break;
            }
            else
                {error_code = BAD_REQ; break;}
        }

        file_reached ? bzero(&recv_buff_f, sizeof recv_buff_f)
                     : bzero(&recv_buf, sizeof recv_buf);

        lopps++;
    }

    if (!file_reached)
        error_code = BAD_REQ;

    if (error_code != OK && req_method == 0)
    {
        write_BAD(ssl);
        goto clean_me;
    }

    char file_dir[200] = "/home/vic/Desktop/ev2/events";
    char *f = strcat(file_dir, map_get(http_req, "url"));
    puts(f);

    if (starts_with_word("/files", map_get(http_req, "url")))
        upload_file(f, "image/jpeg", ssl);
    else if (starts_with_word("/api", map_get(http_req, "url")))
    {
        switch (req_method)
        {
        case GET:
            serve_JSON(ssl, map_get(http_req, "url"));
            break;
        default:
            receive_json(ssl,
                         map_get(http_req, "url"),
                         string_create_copy(json_b->chars));
            break;
        case 0:
            write_BAD(ssl);
            break;
        }
    }
    else if (starts_with_word("/upload", map_get(http_req, "url")))
    {
        if (req_method == POST)
            receive_file(ssl, map_get(http_req, "url"), filename);
    }
    else
        write_404(ssl);

clean_me:

    b = NULL;
    map_destroy(http_req);
    SSL_free(ssl);
    close(sh->sock);
    return 0;
}

SSL_CTX *create_context()
{
    const SSL_METHOD *method;
    SSL_CTX *ctx;

    method = TLS_server_method();

    ctx = SSL_CTX_new(method);

    if (!ctx)
    {
        perror("Unable to create SSL context");
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }

    return ctx;
}


void configure_contex(SSL_CTX *ctx)
{
    if (SSL_CTX_use_certificate_file(ctx, "/home/vic/Desktop/ssl_cookbook/fd.crt", SSL_FILETYPE_PEM) <= 0)
    {
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
        
    }

    if (SSL_CTX_use_PrivateKey_file(ctx, "/home/vic/Desktop/ssl_cookbook/fd.key", SSL_FILETYPE_PEM) <= 0)
    {
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }
}

void accept_connections(int socketfd)
{
    struct sockaddr_storage their_address;
    socklen_t len = sizeof their_address;

    SSL_CTX *ctx;

    ctx = create_context();

    configure_contex(ctx);

    while (1)
    {
        int their_socket;
        SSL *ssl;

        if ((their_socket = accept(socketfd, (struct sockaddr *)&their_address, &len)) < 0)
        {
            //perror("Accept");
            continue;
        }


        ssl = SSL_new(ctx);
        SSL_set_fd(ssl, their_socket);



        if (SSL_accept(ssl) <= 0){
            ERR_print_errors_fp(stderr);
            puts("failed to accept");
            continue;
        }
        else
        {

            ssl_holder * sh = malloc(sizeof(ssl_holder));

            sh->ssl_strct = &ssl;
            sh->sock = their_socket;

            tpool_add_work(thread_pool, handle_request, sh);

            //handle_request(sh);

            //thrd_t t;

            //thrd_create(&t,handle_request,sh);

            //thrd_join(t,NULL);

            puts("Fala sana --------");
        }

        puts("****************************************");

        //SSL_shutdown(ssl);

        puts("============================================");


        
        
    }

    SSL_CTX_free(ctx);
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

        printf("Locust running on port :%d of this computer\n", port);

        socketfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);

        if (socketfd < 0)
        {
            perror("Socket : ");
        }
        else
        {
            setsockopt(socketfd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof yes);
            break;
        }
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
    //cpool = create_conn_pool(5);

    accept_connections(socketfd);

    close(socketfd);
    free(server_info);

    return true;
}
