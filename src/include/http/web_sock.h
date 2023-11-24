#pragma once

#include "http.h"

extern struct pollfd *pfds;
extern int fd_count_g;
extern int fd_size_g;
extern cnd_t poll_condition;
extern mtx_t poll_mutex;

#define GUID "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"
#define BUFFER_SIZE 1024 * 1024

bool validate_WS_connection(map_t *request);

char *createAcceptString(char *ws_key);

unsigned int createIntFromByte(unsigned int *bytes, size_t len);

void *get_in_addr_ws(struct sockaddr *sa);

void add_to_pfds(struct pollfd *pfds[], int newfd, int *fd_count, int *fd_size);

void del_from_pfds(struct pollfd pfds[], int i, int *fd_count);

int startChartSystem(void *v);

void parse_flags(char *bytes,int *fin, int *opcode , int *mask);

void parse_payload_length(char *bytes, int *payloadLength, int *maskStart);

void parse_masking_key(int mask,int mask_start,char *bytes,char *mask_bytes);

void parse_payload(int maskstart,int pay_load_length,char *mask_key,char *bytes,char *decoded_payload);

void encode_message(char *message,size_t message_len,bool is_last,unsigned int opcode,char *encoded_buff,int *enocoded_buff_len);

void send_close_frame(char *client_close_message,int sockfd,int pos);

void send_ping_frame(int sockfd);

void send_pong_frame(char *client_client_ping,int sockfd);