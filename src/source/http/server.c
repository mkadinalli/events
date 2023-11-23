#include "../../include/http/server.h"

#include "endpoints.h"
#include "web_sock.h"
#include "res_builder.h"

#define BUFFER_SIZE 1024 * 1024

int socketfd = 0;

tpool_t *thread_pool = NULL;
conn_pool *cpool = NULL;
int server_fd = -1;
SSL_CTX *server_accept_ctx = NULL;


struct pollfd *pfds = NULL;
int fd_count_g = 0;
int fd_size_g = 0;

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

int handle_request(void *args)
{
    puts("#########################################################");

    int *ss = (int*)(args);
    int ssl = *ss;

    char recv_buf[1] = {0};
    char recv_buff_f[100] = {0};
    int bytes_received;
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

    long content_len = -1;
    long total_received = 0;


    while (true)
    {

        bytes_received = recv(ssl, file_reached ? recv_buff_f : recv_buf, file_reached ? 99 : 1,0);


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
            puts(recv_buff_f);
            printf("comming from %d\n",SSL_get_fd(ssl));

            if (ptr && file_type == IMAGE)
                fwrite(recv_buff_f, 1, bytes_received, ptr);

            if (file_type == JSON)
                    string_concat(json_b, recv_buff_f, bytes_received);

        }

        if (recv_buf[0] == end_of_header[marker])
            marker++;
        else
            marker = 0;

        if(file_reached) total_received += bytes_received;

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
                //write_BAD(ssl);
                break;
            }

            map_print(http_req);

            char *key = createAcceptString(map_get(http_req,"sec-websocket-key"));
            if(strcmp(map_get(http_req,"sec-websocket-key"),"dghlihnhbxbszs6ub25jzq==")){
                puts("==== failde to match===========");
            }else{
                puts("+++++++matched+++++++++");
            }
            response_builder *rs = response_builder_create();
            response_builder_set_code(rs, "101");
            response_builder_set_status_name(rs, "Switching Protocols");
            response_builder_set_header(rs,"Connection","Upgrade");
            response_builder_set_header(rs,"Upgrade","websocket");
            //response_builder_set_header(rs,"sec-websocket-extensions","permessage-deflate");
            response_builder_set_header(rs,"Sec-WebSocket-Accept",key);

            char *resp = response_builder_to_string(rs);
            puts("==================================================");
            if(resp) puts(resp);
            puts("===================================================");

            send(ssl,resp,strlen(resp),0);

            //add_to_pfds(&pfds,SSL_get_fd(ssl),&fd_count_g,&fd_size_g);
            break;
            //close(SSL_get_fd(ssl));



            /*if (map_get_ref(http_req, "content-type") != NULL)
            {
                if (starts_with_word("image/jpeg",map_get_ref(http_req, "content-type")))
                {
                    file_type = IMAGE;
                    sprintf(filename, "./files/image%lu.jpg", (unsigned long)time(NULL));
                    ptr = fopen(filename, "a");
                }
                else if (starts_with_word("application/json",map_get_ref(http_req, "content-type")) ||
                starts_with_word("text/plain",map_get_ref(http_req, "content-type")) )
                    file_type = JSON;
            }

            if (starts_with_word( map_get_ref(http_req, "method"), "GET"))
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


            if(req_method == 0)   {error_code = BAD_REQ; puts("===Method not found====="); break;}

            if(map_get_ref(http_req, "content-length") == NULL && (req_method == POST || req_method == PUT))
            {
                write_BAD(ssl);
                break;
            }

            content_len = strtol(map_get_ref(http_req, "content-length"),NULL,0);

            printf("Content len = %ld\n",content_len);*/
        }

        file_reached ? bzero(&recv_buff_f, sizeof recv_buff_f)
                     : bzero(&recv_buf, sizeof recv_buf);

        if((lopps >= 4096 && !file_reached) || (lopps > (1024 * 1024 * 20)))
            break;

        lopps++;
    }

    goto clean_me;

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

    if (starts_with_word("/files", map_get_ref(http_req, "url")))
        upload_file(f, "image/jpeg", ssl);
    else if (starts_with_word("/api", map_get_ref(http_req, "url")))
    {
        switch (req_method)
        {
        case GET:
            //method_get(ssl, map_get_ref(http_req, "url"));
            break;
        case POST:
            //method_post(ssl,map_get_ref(http_req, "url"),string_create_copy(json_b->chars));
            //write_OK(ssl,"");
            break;

        case PUT:
            //method_put(ssl,map_get_ref(http_req, "url"),string_create_copy(json_b->chars));
            break;

        case DELETE:
            //method_delete(ssl,map_get_ref(http_req, "url"));
            break;

        case OPTIONS:
            write_OK(ssl,"");
            break;

        default:
            write_BAD(ssl);
            break;
        }
    }
    else if (starts_with_word("/upload", map_get_ref(http_req, "url")))
    {
        if (req_method == POST)
            method_post_file(ssl, map_get_ref(http_req, "url"), filename);
    }
    else
        write_404(ssl);

clean_me:

    b = NULL;
    map_destroy(http_req);

    //SSL_free(ssl);
    //close(sh->sock);
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
    if (SSL_CTX_use_certificate_file(ctx, "./config/fd.crt", SSL_FILETYPE_PEM) <= 0)
    {
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }

    if (SSL_CTX_use_PrivateKey_file(ctx, "./config/fd.key", SSL_FILETYPE_PEM) <= 0)
    {
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }
}

void accept_connections(int socketfd)
{
    struct sockaddr_storage their_address;
    socklen_t len = sizeof their_address;

    //server_accept_ctx = create_context();

    //configure_contex(server_accept_ctx);

    ////////////////////////////////////

    struct sockaddr_storage remoteaddr; // Client address
    socklen_t addrlen;

    char buf[BUFFER_SIZE];
    // Buffer for client data
    char remoteIP[INET6_ADDRSTRLEN];
    // Start off with room for 5 connections
    // (We'll realloc as necessary)
    //int fd_count = 0;
    //int fd_size = 5;
    fd_count_g = 0;
    fd_size_g = 5;
    pfds = malloc(sizeof *pfds * fd_size_g);
    // Set up and get a listening socket

    // Add the listener to set
    pfds[0].fd = server_fd;
    pfds[0].events = POLLIN; // Report ready to read on incoming connection
    //fd_count = 1;
    fd_count_g = 1;
    // For the listener
    // Main loop
    SSL *sock_wrap = SSL_new(server_accept_ctx);
    SSL *ssl_;
    //if(sock_wrap == NULL) exit(1);
    //if(pfds == NULL) puts("NULL PFDS");
    puts("ACCEPTING CONNECTIONS");
    for (;;)
    {
        int poll_count = poll(pfds, fd_count_g, -1);
        if (poll_count == -1)
        {
            perror("poll");
            exit(1);
        }
        // Run through the existing connections looking for data to read
        for (int i = 0; i < fd_count_g; i++)
        {
            // Check if someone's ready to read
            if (pfds[i].revents & POLLIN)
            { // We got one!!
                if (pfds[i].fd == server_fd)
                {
                    // If listener is ready to read, handle new connection
                    puts("Reading from server fd");
                    addrlen = sizeof remoteaddr;

                    int newfd = accept(server_fd, (struct sockaddr *)&remoteaddr, &addrlen);

                    if (newfd == -1)
                    {
                        perror("accept");
                    }
                    else
                    {
                        puts("New FD");
                        
                        /*ssl_ = SSL_new(server_accept_ctx);
                        SSL_set_fd(ssl_, newfd);

                        if (SSL_accept(ssl_) <= 0)
                        {
                            continue;
                        }
                        else
                        {

                            ssl_holder *sh = malloc(sizeof(ssl_holder));

                            sh->ssl_strct = &ssl_;
                            sh->sock = newfd;

                            puts("SSL ACCEPT");

                            //tpool_add_work(thread_pool, handle_request, sh);

                            puts("REQEUST HANDLED SUCCESSFULLY");
                        }*/
                        handle_request(&newfd);

                        add_to_pfds(&pfds, newfd, &fd_count_g, &fd_size_g);

                        printf("pollserver: new connection from %s on socket %d\n", inet_ntop(remoteaddr.ss_family, get_in_addr((struct sockaddr *)&remoteaddr), remoteIP, INET6_ADDRSTRLEN), newfd);
                    }
                }
                else
                {
                    int sender_fd = pfds[i].fd;
                    //SSL_set_fd(sock_wrap,sender_fd);
                    // If not the listener, we're just a regular client
                    int nbytes = recv(sender_fd, buf, sizeof buf,0);

                    // decoding staff

                    if (nbytes <= 0)
                    {
                        // Got error or connection closed by client
                        if (nbytes == 0)
                        {
                            // Connection closed
                            printf("pollserver: socket %d hung up\n", sender_fd);
                        }
                        else
                        {
                            perror("recv error");
                        }

                        close(pfds[i].fd); // Bye!
                        del_from_pfds(pfds, i, &fd_count_g);

                        puts("----------fd deleted--------");
                    }
                    else
                    {
                        //puts(buf);
                        int opcode,fin,mask,plen,mask_st;
                        parse_flags(buf,&fin,&opcode,&mask);
                        parse_payload_length(buf,&plen,&mask_st);
                        char key[5] = {0};
                        parse_masking_key(mask,mask_st,buf,key);
                        printf("mask %d fin %d opcode %d length %d mask_start %d\n", mask,fin,opcode,plen,mask_st);
                        char message[BUFFER_SIZE] = { 0 };
                        parse_payload(mask_st,plen,key,buf,message);
                        printf("Decoded message is %s\n",message);
                        // We got some good data from a client

                        char response[BUFFER_SIZE] = {0};
                        int res_len;
                        encode_message("Hello toobbbbbbbbbbbbbbbbbbbbbbbbbbbbereererererbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww",267,true,true,response,&res_len);
                        printf("||||||||Response length is %d",res_len);

                        for (int j = 0; j < fd_count_g; j++)
                        {
                            int dest_fd = pfds[j].fd;
                            //SSL_set_fd(sock_wrap,dest_fd);
                            // Send to everyone!
                            // Except the listener and ourselves
                            if (dest_fd != server_fd /*&& dest_fd != sender_fd*/)
                            {
                                if (send(dest_fd, response, res_len,0) == -1)
                                {
                                    perror("send");
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    /*while (1)
    {




        int their_socket;
        SSL *ssl;

        if ((their_socket = accept(socketfd, (struct sockaddr *)&their_address, &len)) < 0)
        {
            //perror("Accept");
            continue;
        }


        ssl = SSL_new(server_accept_ctx);
        SSL_set_fd(ssl, their_socket);

        if (SSL_accept(ssl) <= 0){
            continue;
        }
        else
        {

            ssl_holder * sh = malloc(sizeof(ssl_holder));

            sh->ssl_strct = &ssl;
            sh->sock = their_socket;

            //tpool_add_work(thread_pool, handle_request, sh);
        }
    }

    SSL_CTX_free(server_accept_ctx);
    */
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
    }

    if ((status = listen(server_fd, BACKLOG)) < 0)
    {
        perror("listen");
    }

    thread_pool = tpool_create(4);

    // tpool_add_work(thread_pool,startChartSystem,NULL);

    cpool = create_conn_pool(5);

    accept_connections(server_fd);

    close(server_fd);
    free(server_info);

    return true;
}
