#include "../include/requests.h"

char *send_http_request(map_t *map,char *url)
{
    //char *PORT = "3000";
    int socketfd;
    struct addrinfo hints;
    struct addrinfo *server_info, *p;

    memset(&hints, 0, sizeof hints);

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    // 0 for success
    int status = getaddrinfo(url, "http", &hints, &server_info);

    if (status != 0)
    {
        fprintf(stderr, "Error : %s\n", gai_strerror(status));
        return NULL;
    }

    char address[INET6_ADDRSTRLEN];
    //int port = 0;

    for (p = server_info; p != NULL; p = p->ai_next)
    {

        if ((socketfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
        {
            perror("client socket");
            continue;
        }

        if ((status = connect(socketfd, p->ai_addr, p->ai_addrlen)) == -1)
        {
            close(socketfd);
            perror("client connect");
            continue;
        }

        break;
    }

    if (p == NULL)
    {
        fprintf(stderr, "Failed to connect\n");
        return NULL;
    }

    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
              address,
              sizeof address);

    printf("Connecting to %s\n",address);

    char recv_buf[10];
    int bytes_received;

    char * message = "GET / HTTP/1.1\r\n";

    //string_t * b = string_create();

    if((send(socketfd,message,strlen(message),0)) == -1)
    {
        perror("send");
        return NULL;
    }

    /*if((bytes_received = recv(socketfd,&recv_buf,1023,0)) == -1)
    {
        perror("recv");
        //printf("%d ",bytes_received);
        return NULL;
    }*/

    while((bytes_received = recv(socketfd,recv_buf,9,0)))
    {
        if(bytes_received == -1)
        {
            perror("recv");
        }

        if(bytes_received == 0)
        {
            break;
        }

        printf("%s",recv_buf);
        bzero(&recv_buf,sizeof recv_buf);
        map_destroy(map);
    }

    printf("%s\n",recv_buf);
    
    //write()
    close(socketfd);
    free(server_info);
    return NULL;
}

void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
    {
        return &(((struct sockaddr_in *)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}
