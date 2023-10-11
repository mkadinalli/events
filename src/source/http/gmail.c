#include "gmail.h"

bool gmail_send_message(char *receiver_email,char *message)
{
    char recvbuff[4096];
    char *cmd, *enc_cmd;
    int r;
    struct sockaddr *addr = NULL;
    int sock = http_client_create_socket("smtp.gmail.com","587",&addr);


    r = recv(sock,recvbuff,BUFFLEN-1,0);

    if(r <= 0)
        return false;

    cmd = "EHLO smtp.gmail.com\r\n";

    r = send(sock,cmd,strlen(cmd),0);

    if(r <= 0)
        return false;

    bzero(recvbuff,sizeof recvbuff);

    r = recv(sock,recvbuff,BUFFLEN-1,0);

    if(r <= 0)
        return false;

    cmd = "STARTTLS\r\n";
    
    r = send(sock,cmd,strlen(cmd),0);

    if(r <= 0)
        return false;

    bzero(recvbuff,sizeof recvbuff);

    r = recv(sock,recvbuff,BUFFLEN-1,0);

    if(r <= 0)
        return false;

    OpenSSL_add_all_algorithms();
    ERR_load_BIO_strings();
    ERR_load_CRYPTO_strings();
    SSL_load_error_strings();

    SSL_CTX * ctx = SSL_CTX_new(SSLv23_client_method());

    if(ctx == NULL)
        return false;

    SSL *ssl = SSL_new(ctx);

    if(ssl == NULL)
    {
        SSL_CTX_free(ctx);
        return false;
    }

    SSL_set_fd(ssl,sock);

    r = SSL_connect(ssl);

    if(r < 0)
        goto exit_with_error;

    cmd = "EHLO smtp.gmail.com\r\n";
    
    r = SSL_write(ssl,cmd,strlen(cmd));

    if(r <= 0)
        goto exit_with_error;

    bzero(recvbuff,sizeof recvbuff);

    r = SSL_read(ssl,recvbuff,BUFFLEN-1);

    if(r <= 0)
        goto exit_with_error;


    cmd = "AUTH LOGIN\r\n";
    
    r = SSL_write(ssl,cmd,strlen(cmd));

    if(r <= 0)
        goto exit_with_error;

    bzero(recvbuff,sizeof recvbuff);

    r = SSL_read(ssl,recvbuff,BUFFLEN-1);

    if(r <= 0)
        goto exit_with_error;

    cmd = "vycnjagi@gmail.com";

    cmd = base64_encode(cmd,strlen(cmd));

    cmd = strcat(cmd,"\r\n");

    r = SSL_write(ssl,cmd,strlen(cmd));


    if(r <= 0)
        goto exit_with_error;

    bzero(recvbuff,sizeof recvbuff);

    r = SSL_read(ssl,recvbuff,BUFFLEN-1);

    if(r <= 0)
        goto exit_with_error;

    cmd = "zmlw avhn mjun mkss";
    cmd = base64_encode(cmd,strlen(cmd));

    cmd = strcat(cmd,"\r\n");

    r = SSL_write(ssl,cmd,strlen(cmd));

    if(r <= 0)
        goto exit_with_error;

    bzero(recvbuff,sizeof recvbuff);

    r = SSL_read(ssl,recvbuff,BUFFLEN-1);

    if(r <= 0)
        goto exit_with_error;

    cmd = "MAIL FROM: <vycnjagi@gmail.com>\r\n";
    
    r = SSL_write(ssl,cmd,strlen(cmd));

    if(r <= 0)
        goto exit_with_error;

    bzero(recvbuff,sizeof recvbuff);

    r = SSL_read(ssl,recvbuff,BUFFLEN-1);

    if(r <= 0)
        goto exit_with_error;

    cmd = "RCPT TO: <%s>\r\n";

    char rcmd[4096];

    sprintf(rcmd,cmd,receiver_email);
    
    r = SSL_write(ssl,rcmd,strlen(rcmd));

    if(r <= 0)
        goto exit_with_error;

    bzero(recvbuff,sizeof recvbuff);

    r = SSL_read(ssl,recvbuff,BUFFLEN-1);

    if(r <= 0)
        goto exit_with_error;

    cmd = "DATA\r\n";
    
    r = SSL_write(ssl,cmd,strlen(cmd));

    if(r <= 0)
        goto exit_with_error;

    bzero(recvbuff,sizeof recvbuff);

    r = SSL_read(ssl,recvbuff,BUFFLEN-1);

    if(r <= 0)
        goto exit_with_error;

    cmd = "Date: Tue, 09 oct 2023 16:26:00\r\nFrom: vycnjagi@gmail.com\r\nSubject: Hello world\r\nTo: %s\r\nBill:\r\n%s\nvic.\r\n.\r\n";
    
    bzero(rcmd,sizeof rcmd);
    
    sprintf(rcmd,cmd,receiver_email,message);

    r = SSL_write(ssl,rcmd,strlen(rcmd));

    if(r <= 0)
        goto exit_with_error;

    bzero(recvbuff,sizeof recvbuff);

    r = SSL_read(ssl,recvbuff,BUFFLEN-1);

    if(r <= 0)
        goto exit_with_error;

    cmd = "QUIT\r\n";
    
    r = SSL_write(ssl,cmd,strlen(cmd));

    if(r <= 0)
        goto exit_with_error;

    bzero(recvbuff,sizeof recvbuff);

    r = SSL_read(ssl,recvbuff,BUFFLEN-1);

    if(r <= 0)
        goto exit_with_error;


    SSL_free(ssl);
    SSL_CTX_free(ctx);
    close(sock);
    return true;


    exit_with_error:
    SSL_free(ssl);
    SSL_CTX_free(ctx);
    close(sock);
    return false;
}