#include "web_sock.h"

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
    string_concat(conc,GUID,37);

    unsigned char *sha_output = malloc(SHA_DIGEST_LENGTH * sizeof(char)) ;

    SHA1((const unsigned char *) conc->chars , strlen(conc->chars), sha_output);

    char *x_64ret = base64_encode(sha_output,SHA_DIGEST_LENGTH);
    
    string_destroy(conc);
    free(sha_output);

    puts(x_64ret);

    return x_64ret;
    
}


int createIntFromByte(int bytes[], size_t len){
    int result = bytes[0];

    result = result << 8;

    for(int i = 1; i < len; i++){
        result = result | bytes[i];
        if(i != len - 1) result = result << 8;
    }

    return result;
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

