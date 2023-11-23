#include "web_sock.h"

//struct pollfd *pfds = NULL;
//int fd_count = 0;
//int fd_size = 0;

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

void add_to_pfds(struct pollfd *pfds[], int newfd, int *fd_cnt, int *fd_sz)
{
    // If we don't have room, add more space in the pfds array
    if (*fd_cnt == *fd_sz)
    {
        *fd_sz *= 2; // Double it
        *pfds = realloc(*pfds, sizeof(**pfds) * (*fd_sz));
    }
    (*pfds)[*fd_cnt].fd = newfd;
    (*pfds)[*fd_cnt].events = POLLIN; // Check ready-to-read
    (*fd_cnt)++;
    puts("ADDED ONE SOCKET FD");
    printf("%d new fd is %d\n",*fd_cnt,newfd);
}

void del_from_pfds(struct pollfd pfds[], int i, int *fd_cnt)
{

    pfds[i] = pfds[*fd_cnt - 1];
    (*fd_cnt)--;
}

/**
int startChartSystem(void *v)
{
    puts("Chart SYstem online");
    if(v != NULL){}

    fd_size = 5;
    pfds = malloc(sizeof *pfds * fd_size);
    char buf[256];
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
                if(pfds[i].fd != server_fd)
                // We got one!!
 
                {
                    puts("ONE FD READY TO READ");
                    printf("%d\n",fd_count);
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
                        puts("_______Data___________");
                        puts(buf);
                        puts("________end data______________");
                        for (int j = 0; j < fd_count; j++)
                        {
                            // Send to everyone!
                            int dest_fd = pfds[j].fd;
                            // Except the listener and ourselves
                            if (dest_fd != server_fd && dest_fd != sender_fd)
                            {
                                if (send(dest_fd, buf, nbytes, 0) == -1)
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
}*/

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

void parse_flags(char *bytes,int *fin, int *opcode , int *mask)
{
    *fin = bytes[0] & 0b10000000;
    *opcode = bytes[0] & 0b1111;

    *mask = bytes[1] & 0b10000000;
}

void parse_payload_length(char *bytes, int *payloadLength, int *maskStart)
{
    int payload_length = bytes[1] & 0b1111111;

    int mask_key_start = 2;

    if(payload_length == 126){
        int b[] = { 
            bytes[2], 
            bytes[3]
         };
        payload_length = createIntFromByte(b,2);
        mask_key_start = 4;
    }else if(payload_length == 127){
        int b[] = {
            bytes[2],
            bytes[3],
            bytes[4],
            bytes[5],
            bytes[6],
            bytes[7],
            bytes[8]
        };

        payload_length = createIntFromByte(b,7);
        mask_key_start = 10;
    }

    *payloadLength = payload_length;
    *maskStart = mask_key_start;
}

void parse_masking_key(int mask,int mask_start,char *bytes,char *mask_bytes)
{
    if(!mask){
        return;
    }

    mask_bytes[0] = bytes[mask_start];
    mask_bytes[1] = bytes[mask_start + 1];
    mask_bytes[2] = bytes[mask_start + 2];
    mask_bytes[3] = bytes[mask_start + 3];
}


void parse_payload(int maskstart,int pay_load_length,char *mask_key,char *bytes,char *decoded_payload)
{
    int payload_start = maskstart + 4;

    //if( maskstart != 2) payload_start = maskstart + 4;

    char *encoded_payload = bytes+payload_start;

    for(int i = 0; i < pay_load_length; i++){
        decoded_payload[i] =  (char)encoded_payload[i] ^ mask_key[i % 4];
    }
}


void encode_message(char *message,size_t message_len,bool is_last,bool is_text,char *encoded_buff)
{
    int first_byte = 0b00000000;
    int second_byte = 0b00000000;

    if(is_last){
        first_byte = first_byte | 0b10000000;
    }

    encoded_buff[0] = first_byte;

    int message_start = 2;

    /**
     * 11111111 11111111 11111111 11111111
     * 
     * 00000000 11111111 11111111 11111111 (rshift 8  & pow(32,2)) >> 16
     * 
     * 00000000 00000000 11111111 11111111 (rshift 16 & pow(32,2)) >> 8
     * 
     * 00000000 00000000 00000000 11111111 (rshift 24 & pow(32,2)) >> 0
     * 
     * 
    */


    if(message_len < 256){
        second_byte = second_byte | message_len;

        encoded_buff[1] = second_byte;
    }else if(message_len >= 256 && message_len < 65536){
        second_byte = second_byte | 126;

        encoded_buff[1] = second_byte;
        encoded_buff[2] = (message_len & 0b1111111111111111) >> 8;
        encoded_buff[3] = (message_len & 0b0000000011111111);

        message_start = 4;

    }else{
        second_byte = second_byte | 127;

        encoded_buff[1] = second_byte;
        
        encoded_buff[2] = ((message_len >> 56) & 65535) >> 0;
        encoded_buff[3] = ((message_len >> 48) & 65535) >> 8;
        encoded_buff[4] = ((message_len >> 40) & 65535) >> 16;
        encoded_buff[5] = ((message_len >> 32) & 65535) >> 24;
        encoded_buff[6] = ((message_len >> 24) & 65535) >> 32;
        encoded_buff[7] = ((message_len >> 16) & 65535) >> 40;
        encoded_buff[8] = ((message_len >> 8) & 65535) >> 48;
        encoded_buff[9] = ((message_len >> 0) & 65535) >> 56;

        message_start = 10;
        
    }

    int j = 0;
    for(int i = message_start; j < message_len; i++){
        encoded_buff[i] = message[j];
        j++;
    }
}

