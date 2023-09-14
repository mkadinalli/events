#include "../include/server.h"
#include "../include/pool.h"
#include "../include/lib.h"
#include "../include/map.h"
#include "../include/string_buffer.h"

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

    char buff[1024] = {0};

    int *th = (int *)args;
    int x, their_socket = *th;
    if ((x = recv(their_socket, buff, 1024, 0)) == 0)
    {
        exit(1);
    }

    hashtable_t *http_req = parse_http_req(buff, strlen(buff));

    if (strcmp(hashtable_get(http_req, "method"), "GET") != 0)
    {
        puts("bad method");
    }
    else
    {
        serve_client(their_socket, hashtable_get(http_req, "url"));
    }

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
