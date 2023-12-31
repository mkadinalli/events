#include "web_sock.h"

struct pollfd *pfds = NULL;
int fd_count_g = 0;
int fd_size_g = 0;

cnd_t poll_condition;
mtx_t poll_mutex;
bool keep_chat_alive = true;
list_t *pfd_ids;

bool validate_WS_connection(map_t *request)
{
    if (request == NULL)
    {
        return false;
    }

    bool isValid = false;

    if (strcmp(map_get_ref(request, "sec-websocket-version"), "13"))
    {
        isValid = false;
    }
    else
    {
        isValid = true;
    }

    return isValid;
}

char *createAcceptString(char *input)
{
    string_t *conc = string_create_from_string(input);
    string_concat(conc, GUID, strlen(GUID));

    unsigned char *sha_output = malloc(SHA_DIGEST_LENGTH);

    SHA1((const unsigned char *)conc->chars, strlen(conc->chars), sha_output);

    unsigned char *x_64ret = base64_encode(sha_output, SHA_DIGEST_LENGTH);

    string_destroy(conc);
    free(sha_output);

    return (char *)x_64ret;
}

unsigned int createIntFromByte(unsigned int bytes[], size_t len)
{
    unsigned int result = bytes[0];

    result = result << 8;

    for (size_t i = 1; i < len; i++)
    {
        result = result | bytes[i];
        if (i != len - 1)
            result = result << 8;
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
    mtx_lock(&poll_mutex);
    if (*fd_cnt == *fd_sz)
    {
        *fd_sz *= 2; 
        *pfds = realloc(*pfds, sizeof(**pfds) * (*fd_sz));
    }
    (*pfds)[*fd_cnt].fd = newfd;
    (*pfds)[*fd_cnt].events = POLLIN;
    (*fd_cnt)++;
    mtx_unlock(&poll_mutex);
    cnd_signal(&poll_condition);
}

void del_from_pfds(struct pollfd pfds[], int i, int *fd_cnt)
{
    mtx_lock(&poll_mutex);
    pfds[i] = pfds[*fd_cnt - 1];
    (*fd_cnt)--;
    mtx_unlock(&poll_mutex);
}

int startChartSystem(void *v)
{
    keep_chat_alive = true;
    puts("Chart SYstem online");
    if (v != NULL)
    {
    }

    cnd_init(&poll_condition);
    mtx_init(&poll_mutex, 0);

    char buf[BUFFER_SIZE];

    fd_count_g = 0;
    fd_size_g = 5;
    pfds = malloc(sizeof *pfds * fd_size_g);

    while (keep_chat_alive)
    {
        if (fd_count_g == 0)
        {
            mtx_lock(&poll_mutex);
            while (fd_count_g == 0)
                cnd_wait(&poll_condition, &poll_mutex);

            mtx_unlock(&poll_mutex);
        }

        if (!keep_chat_alive)
            break;

        int poll_count = poll(pfds, fd_count_g, -1);

        if (poll_count == -1)
        {
            perror("poll");
            exit(1);
        }

        for (int i = 0; i < fd_count_g; i++)
        {
            if (pfds[i].revents & POLLIN)
            {
                int sender_fd = pfds[i].fd;
                int nbytes = recv(sender_fd, buf, sizeof buf, 0);

                if (nbytes <= 0)
                {
                    if (nbytes == 0)
                    {
                        printf("pollserver: socket %d hung up\n", sender_fd);
                    }
                    else
                    {
                        perror("recv error");
                    }

                    close(pfds[i].fd); // Bye!
                    del_from_pfds(pfds, i, &fd_count_g);
                }
                else
                {
                    // puts(buf);
                    int opcode, fin, /*whether set*/ mask, plen, mask_st;

                    parse_flags(buf, &fin, &opcode, &mask);

                    switch (opcode)
                    {
                    case 0x8:
                        puts("Close frame received--------------------------");
                        send_close_frame(buf, sender_fd, i);
                        continue;
                        break;

                    case 0x9:
                        puts("ping frame received-----------------");
                        send_pong_frame(buf, sender_fd);
                        continue;
                        break;

                    case 0xA:
                        continue;
                        break;

                    default:
                        puts("normal frame received-----------------------");
                        break;
                    }

                    parse_payload_length(buf, &plen, &mask_st);

                    char key[5] = {0};

                    parse_masking_key(mask, mask_st, buf, key);

                    printf("mask %d fin %d opcode %d length %d mask_start %d\n", mask, fin, opcode, plen, mask_st);

                    char message[BUFFER_SIZE] = {0};

                    parse_payload(mask_st, plen, key, buf, message);

                    printf("Decoded message is %s\n", message);

                    char response[BUFFER_SIZE] = {0};
                    int res_len;
                    encode_message("Hello", 5, true, 1, response, &res_len);
                    printf("||||||||Response length is %d", res_len);

                    //parse json -> asign id to fd map.

                    /*for (int j = 0; j < fd_count_g; j++)
                    {
                        int dest_fd = pfds[j].fd;
                        if (dest_fd != server_fd && dest_fd != sender_fd)
                        {
                            if (send(dest_fd, response, res_len, 0) == -1)
                            {
                                perror("send");
                            }
                        }
                    }*/
                }
            }
        }
    }
    puts("Chat destroyed");
    return 0;
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

void parse_flags(char *bytes, int *fin, int *opcode, int *mask)
{
    *fin = bytes[0] & 0b10000000;
    *opcode = bytes[0] & 0b1111;

    *mask = bytes[1] & 0b10000000;
}

void parse_payload_length(char *bytes, int *payloadLength, int *maskStart)
{
    unsigned int payload_length = bytes[1] & 0b1111111;

    int mask_key_start = 2;

    if (payload_length == 126)
    {
        unsigned int b[] = {
            (unsigned char)bytes[2],
            (unsigned char)bytes[3]};
        payload_length = createIntFromByte(b, 2);
        mask_key_start = 4;
    }
    else if (payload_length == 127)
    {
        unsigned int b[] = {
            (unsigned char)bytes[2],
            (unsigned char)bytes[3],
            (unsigned char)bytes[4],
            (unsigned char)bytes[5],
            (unsigned char)bytes[6],
            (unsigned char)bytes[7],
            (unsigned char)bytes[8],
            (unsigned char)bytes[9]};

        payload_length = createIntFromByte(b, 8);
        mask_key_start = 10;
    }

    *payloadLength = payload_length;
    *maskStart = mask_key_start;
}

void parse_masking_key(int mask, int mask_start, char *bytes, char *mask_bytes)
{
    if (!mask)
    {
        return;
    }

    mask_bytes[0] = bytes[mask_start];
    mask_bytes[1] = bytes[mask_start + 1];
    mask_bytes[2] = bytes[mask_start + 2];
    mask_bytes[3] = bytes[mask_start + 3];
}

void parse_payload(int maskstart, int pay_load_length, char *mask_key, char *bytes, char *decoded_payload)
{
    int payload_start = maskstart + 4;

    if (mask_key == NULL)
    {
        payload_start = maskstart;

        int j = 0;
        for (int i = payload_start; j < pay_load_length; i++)
        {
            decoded_payload[j] = (unsigned char)bytes[i];
            j++;
        }
        return;
    }

    int j = 0;
    for (int i = payload_start; j < pay_load_length; i++)
    {
        decoded_payload[j] = (char)bytes[i] ^ mask_key[j % 4];
        j++;
    }
}

void encode_message(char *message, size_t message_len, bool is_last, unsigned int opcode, char *encoded_buff, int *encoded_buff_len)
{
    int first_byte = 0b00000000;
    int second_byte = 0b00000000;

    if (is_last)
    {
        first_byte = first_byte | 0b10000000;
    }

    first_byte = first_byte | opcode;

    encoded_buff[0] = first_byte;

    int message_start = 2;

    if (message_len < 126)
    {
        second_byte = second_byte | message_len;

        encoded_buff[1] = second_byte;
    }
    else if (message_len >= 126 && message_len < 0b10000000000000000)
    {
        second_byte = second_byte | 126;

        encoded_buff[1] = second_byte;
        encoded_buff[2] = (message_len & 0b1111111111111111) >> 8;
        encoded_buff[3] = (message_len & 0b0000000011111111);

        message_start = 4;
    }
    else
    {
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

        message_start = 10;
    }

    unsigned int j = 0;
    *encoded_buff_len = message_start;

    for (int i = message_start; j < message_len; i++)
    {
        encoded_buff[i] = message[j];
        j++;
    }

    *encoded_buff_len += j;
}

void send_close_frame(char *client_close_message, int sockfd, int pos)
{

    int plen, mask_st;
    parse_payload_length(client_close_message, &plen, &mask_st);

    char key[5] = {0};

    parse_masking_key(1 /*always 1 from client*/, mask_st, client_close_message, key); 

    char message[BUFFER_SIZE] = {0};
    parse_payload(mask_st, plen, key, client_close_message, message);

    // int status_code = 0; later use
    size_t response_size = 0;
    char response[10] = {0};

    if (plen > 1)
    {
        /** unsigned int status[2] = {
         *  (unsigned char) message[0],
         * (unsigned char) message[1]
         *};
         */
        // status_code = createIntFromByte(status,2);

        response[0] = message[0];
        response[1] = message[1];
        response_size = 2;
    }

    char close_frame[BUFFER_SIZE] = {0};
    int close_frame_size;

    encode_message(response, response_size, true, 8, close_frame, &close_frame_size);

    send(sockfd, close_frame, close_frame_size, 0);

    del_from_pfds(pfds, pos, &fd_count_g);
    char fake_buff[10];
    int fake_recv;

    shutdown(sockfd, SHUT_WR);

    while ((fake_recv = recv(sockfd, fake_buff, 10, 0)) != 0)
    {
    }

    close(sockfd);
}

void send_pong_frame(char *client_ping_message, int sockfd)
{

    int plen = 0, mask_st;
    parse_payload_length(client_ping_message, &plen, &mask_st);

    char key[5] = {0};

    parse_masking_key(1 /*always 1 from client*/, mask_st, client_ping_message, key);

    char message[BUFFER_SIZE] = {0};
    parse_payload(mask_st, plen, key, client_ping_message, message);

    char pong_frame[BUFFER_SIZE] = {0};
    int pong_frame_size;

    encode_message(message, plen, true, 8, pong_frame, &pong_frame_size);

    send(sockfd, pong_frame, pong_frame_size, 0);
}
