#include "web_sock.h"

struct pollfd *pfds = NULL;
int fd_count = 0;
int fd_size = 0;

bool validate_WS_connection(map_t *request){
    if(request == NULL){
        return false;
    }

    bool isValid = false;

    if(strcmp(map_get_ref(request,"sec-websocket-version") , "13")){
        isValid = false;
    }else{ isValid = true; }

    return isValid;
}


char *createAcceptString(char *input){
    string_t *conc = string_create_from_string(input);
    string_concat(conc,GUID,strlen(GUID));

    unsigned char *sha_output = malloc(SHA_DIGEST_LENGTH) ;

    SHA1((const unsigned char *) conc->chars , strlen(conc->chars), sha_output);

    unsigned char *x_64ret = base64_encode(sha_output,SHA_DIGEST_LENGTH);
    
    string_destroy(conc);
    free(sha_output);

    //puts(x_64ret);

    return (char *)x_64ret;
    
}


int createIntFromByte(int bytes[], size_t len){
    int result = bytes[0];

    result = result << 8;

    for(size_t i = 1; i < len; i++){
        result = result | bytes[i];
        if(i != len - 1) result = result << 8;
    }

    return result;
}


void *get_in_addr_ws(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
    {
        return &(((struct sockaddr_in *)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

void add_to_pfds(struct pollfd *pfds[], int newfd, int *fd_count, int *fd_size)
{
    // If we don't have room, add more space in the pfds array
    if (*fd_count == *fd_size)
    {
        *fd_size *= 2; // Double it
        *pfds = realloc(*pfds, sizeof(**pfds) * (*fd_size));
    }
    (*pfds)[*fd_count].fd = newfd;
    (*pfds)[*fd_count].events = POLLIN; // Check ready-to-read
    (*fd_count)++;
}

void del_from_pfds(struct pollfd pfds[], int i, int *fd_count)
{

    pfds[i] = pfds[*fd_count - 1];
    (*fd_count)--;
}


int startChartSystem(void *v)
{
    if(v != NULL){}

    fd_size = 5;
    pfds = malloc(sizeof *pfds * fd_size);
    // Add the listener to set
    pfds[0].fd = server_fd;
    pfds[0].events = POLLIN; // Report ready to read on incoming connection
    fd_count = 1;            // For the listener
    // Main loop
    for (;;)
    {
        int poll_count = poll(pfds, fd_count, -1);
        if (poll_count == -1)
        {
            perror("poll");
            exit(1);
        }
        // Run through the existing connections looking for data to read
        for (int i = 0; i < fd_count; i++)
        {
            // Check if someone's ready to read
            if (pfds[i].revents & POLLIN)
            { 
                if(pfds[i].fd != server_fd) puts("============ A socket is ready to read ==============");
                // We got one!!
                /*if (pfds[i].fd == listener)
                {
                    // If listener is ready to read, handle new connection
                    addrlen = sizeof remoteaddr;
                    newfd = accept(listener, (struct sockaddr *)&remoteaddr, &addrlen);

                    if (newfd == -1)
                    {
                        perror("accept");
                    }
                    else

                        


                        {
                        add_to_pfds(&pfds, newfd, &fd_count, &fd_size);
                        printf("pollserver: new connection from %s on "
                               "socket %d\n",
                               inet_ntop(remoteaddr.ss_family,
                                         get_in_addr((struct sockaddr *)&remoteaddr),
                                         remoteIP, INET6_ADDRSTRLEN),
                               newfd);
                        }
                }
                else
                {
                    // If not the listener, we're just a regular client
                    int nbytes = recv(pfds[i].fd, buf, sizeof buf, 0);

                    // decoding staff

                    int sender_fd = pfds[i].fd;
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
                            perror("recv");
                        }
                        close(pfds[i].fd); // Bye!
                        del_from_pfds(pfds, i, &fd_count);
                    }
                    else
                    {
                        // We got some good data from a client
                        for (int j = 0; j < fd_count; j++)
                        {
                            // Send to everyone!
                            int dest_fd = pfds[j].fd;
                            // Except the listener and ourselves
                            if (dest_fd != listener && dest_fd != sender_fd)
                            {
                                if (send(dest_fd, buf, nbytes, 0) == -1)
                                {
                                    perror("send");
                                }
                            }
                        }
                    }
                }*/
            }
        }
    }
}

/**
 * 
 * 
      0                   1                   2                   3
      0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
     +-+-+-+-+-------+-+-------------+-------------------------------+
     |F|R|R|R| opcode|M| Payload len |    Extended payload length    |
     |I|S|S|S|  (4)  |A|     (7)     |             (16/64)           |
     |N|V|V|V|       |S|             |   (if payload len==126/127)   |
     | |1|2|3|       |K|             |                               |
     +-+-+-+-+-------+-+-------------+ - - - - - - - - - - - - - - - +
     |     Extended payload length continued, if payload len == 127  |
     + - - - - - - - - - - - - - - - +-------------------------------+
     |                               |Masking-key, if MASK set to 1  |
     +-------------------------------+-------------------------------+
     | Masking-key (continued)       |          Payload Data         |
     +-------------------------------- - - - - - - - - - - - - - - - +
     :                     Payload Data continued ...                :
     + - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - +
     |                     Payload Data continued ...                |
     +---------------------------------------------------------------+
 * 
 * 
*/

