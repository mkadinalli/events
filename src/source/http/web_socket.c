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


unsigned int createIntFromByte(unsigned int bytes[], size_t len){
    unsigned int result = bytes[0];

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
    unsigned int payload_length = bytes[1] & 0b1111111;

    int mask_key_start = 2;

    if(payload_length == 126){
        unsigned int b[] = { 
            (unsigned char) bytes[2], 
            (unsigned char) bytes[3]
         };
        payload_length = createIntFromByte(b,2);
        mask_key_start = 4;
        puts("*********************************************************************************");
        printf("llllllllllllll -> %d\n",payload_length);
    }else if(payload_length == 127){
        puts("===============================================================================");
        unsigned int b[] = {
            (unsigned char) bytes[2],
            (unsigned char) bytes[3],
            (unsigned char) bytes[4],
            (unsigned char) bytes[5],
            (unsigned char) bytes[6],
            (unsigned char) bytes[7],
            (unsigned char) bytes[8],
            (unsigned char) bytes[9]
        };

        payload_length = createIntFromByte(b,8);
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
    

    if( mask_key == NULL ){
        puts("MASK IS NULL............................");
        payload_start = maskstart;
        
        int j = 0;
        for(int i = payload_start; j < pay_load_length; i++){
            decoded_payload[j] =  (unsigned char)bytes[i];
            printf("[%d] %c\n",j,decoded_payload[j]);
            j++;
        }
        return;
    }

    int j = 0;
    for(int i = payload_start; j < pay_load_length; i++){
        decoded_payload[j] =  (char)bytes[i] ^ mask_key[j % 4];
        printf("[%d] %c\n",j,decoded_payload[j]);
        j++;
    }
}


void encode_message(char *message,size_t message_len,bool is_last,unsigned int opcode,char *encoded_buff,int *encoded_buff_len)
{
    int first_byte = 0b00000000;
    int second_byte = 0b00000000;

    if(is_last){
        first_byte = first_byte | 0b10000000;
    }

    first_byte = first_byte | opcode;

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


    if(message_len < 126){
        second_byte = second_byte | message_len;

        encoded_buff[1] = second_byte;
    }else if(message_len >= 126 && message_len < 0b10000000000000000){
        second_byte = second_byte | 126;

        encoded_buff[1] = second_byte;
        encoded_buff[2] = (message_len & 0b1111111111111111) >> 8;
        encoded_buff[3] = (message_len & 0b0000000011111111);

        message_start = 4;

    }else{
        second_byte = second_byte | 127;

        encoded_buff[1] = second_byte;

        unsigned long long len = (unsigned long long)message_len;
        
        encoded_buff[2] = (len & 0b1111111100000000000000000000000000000000000000000000000000000000) >> 56;
        encoded_buff[3] = (len & 0b0000000011111111000000000000000000000000000000000000000000000000) >> 48;
        encoded_buff[4] = (len & 0b0000000000000000111111110000000000000000000000000000000000000000) >> 40;
        encoded_buff[5] = (len & 0b0000000000000000000000001111111100000000000000000000000000000000) >> 32;
        encoded_buff[6] = (len & 0b0000000000000000000000000000000011111111000000000000000000000000) >> 24;
        encoded_buff[7] = (len & 0b0000000000000000000000000000000000000000111111110000000000000000) >> 16;
        encoded_buff[8] = (len & 0b0000000000000000000000000000000000000000000000001111111100000000) >> 8;
        encoded_buff[9] = (len & 0b0000000000000000000000000000000000000000000000000000000011111111); 

        for(int i = 2; i < 10; i++){
            printf("**int  %d  ** char %c\n",encoded_buff[i],encoded_buff[i]);
        }

        message_start = 10;
        
    }

    unsigned int j = 0;
    *encoded_buff_len = message_start;

    for(int i = message_start; j < message_len; i++){
        encoded_buff[i] = message[j];
        j++;
    }

    *encoded_buff_len += j;

}

void send_close_frame(char *client_close_message,int sockfd,int pos){
    

    int plen,mask_st;
    parse_payload_length(client_close_message,&plen,&mask_st);

    char key[5] = {0};

    parse_masking_key(1/*always 1 from client*/,mask_st,client_close_message,key);

    printf("length %d mask_start %d\n",plen,mask_st);

    char message[BUFFER_SIZE] = { 0 };
    parse_payload(mask_st,plen,key,client_close_message,message);

    //int status_code = 0; later use
    size_t response_size = 0;
    char response[10] = {0};

    if(plen > 1){
       /** unsigned int status[2] = {
          *  (unsigned char) message[0],
           * (unsigned char) message[1]
            *};
        */
        //status_code = createIntFromByte(status,2);

        response[0] = message[0];
        response[1] = message[1];
        response_size = 2;
    }

    char close_frame[BUFFER_SIZE] = {0};
    int close_frame_size;

    encode_message(response,response_size,true,8,close_frame,&close_frame_size);

    if(send(sockfd,close_frame,close_frame_size,0) > 0)
    {
        puts("Sent successfully");
    }

    del_from_pfds(pfds,pos,&fd_count_g);
    char fake_buff[10];
    int fake_recv;

    shutdown(sockfd,SHUT_WR);

    while((fake_recv = recv(sockfd,fake_buff,10,0)) != 0){}

    close(sockfd);


    printf("Decoded message is %s\n",message);
}


void send_pong_frame(char *client_ping_message,int sockfd){
        

    int plen = 0,mask_st;
    parse_payload_length(client_ping_message,&plen,&mask_st);

    char key[5] = {0};

    parse_masking_key(1/*always 1 from client*/,mask_st,client_ping_message,key);

    printf("length %d mask_start %d\n",plen,mask_st);

    char message[BUFFER_SIZE] = { 0 };
    parse_payload(mask_st,plen,key,client_ping_message,message);

    char pong_frame[BUFFER_SIZE] = {0};
    int pong_frame_size;

    encode_message(message,plen,true,8,pong_frame,&pong_frame_size);

    if(send(sockfd,pong_frame,pong_frame_size,0) > 0)
    {
        puts("Sent successfully");
    }


    printf("Decoded message is %s\n",message);
}

