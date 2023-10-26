#include "../../include/lib/ev_strings.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>


char *string_create_mpesa_auth(char *consumer_key,char *secret_key)
{
    char merged[1024];
    sprintf(merged,"%s:%s",consumer_key,secret_key);
    char *enc = (char *)base64_encode((unsigned char *)merged,strlen(merged));

    string_t * ss = string_create_from_string("Basic ");

    string_concat(ss,enc,strlen(enc));

    char *tmp = ss->chars;

    free(ss);
    free(enc);

    return tmp;  
}

char *string_create_copy(char *str)
{
    int len = strlen(str) + 1;
    char *res = malloc(sizeof(char) * len);
    strcpy(res, str);
    return res;
}

char *string_cover(char *str){
    size_t len = strlen(str);
    char *newstr = malloc(sizeof(char) * (len+3));

    newstr[0] = '\'';

    for(size_t i = 1; i < len+1;i++)
    {
        newstr[i] = str[i-1];
    }

    newstr[len+1] = '\'';
    newstr[len+2] = '\0';

    return newstr;
}


bool starts_with_word(char *word, char *str)
{
    bool truth = true;

    if (word == NULL || str == NULL)
        return false;

    if (strlen(word) > strlen(str))
        return false;

    for (size_t i = 0; i < strlen(word); i++)
    {
        if (word[i] != str[i])
            truth = false;
    }

    return truth;
}

char *string_removechar(char c, char *buffer, size_t len)
{
    string_t *temp = string_create();

    for (size_t i = 0; i < len; i++)
    {
        if (buffer[i] == c)
        {
            continue;
        }

        string_append(temp, buffer[i]);
    }
    string_append(temp, '\0');
    char *chars = temp->chars;
    free(temp);
    return chars;
}

char *string_replacechar(char previous, char current, char *buffer, size_t len)
{
    if(buffer == NULL) return NULL;
    
    string_t *temp = string_create();
    for (size_t i = 0; i < len; i++)
    {
        if (buffer[i] == previous)
        {
            string_append(temp, current);
     
            continue;
        }

        string_append(temp, buffer[i]);
    }
    string_append(temp, '\0');
    char *chars =string_create_copy(temp->chars);
    string_destroy(temp);
    return chars;
}

char *string_removechar_at(unsigned int pos, char *buffer, size_t len)
{
    string_t *temp = string_create();
    for (size_t i = 0; i < len; i++)
    {
        if (i == pos)
        {
            continue;
        }

        string_append(temp, buffer[i]);
    }
    string_append(temp, '\0');
    char *chars = temp->chars;
    free(temp);
    return chars;
}

int string_last_index_of(char c, char *str, unsigned int len)
{
    int pos = -1;
    int i;
    for (i = len - 1; i > -1; i--)
    {
        if (str[i] == c)
        {
            pos = i;
            break;
        };
    }
    return pos;
}

char *string_sub(unsigned int start, unsigned int stop, char *buffer, unsigned int len)
{
    string_t *temp = string_create();
    for (unsigned int i = 0; i < len; i++)
    {
        if (i >= start && i <= stop)
        {
            string_append(temp, buffer[i]);
        }

        continue;
    }
    string_append(temp, '\0');
    char *chars = temp->chars;
    free(temp);
    return chars;
}

bool endswith(char c, char *str)
{
    if (str[strlen(str) - 1] == c)
        return true;
    return false;
}

bool startswith(char c, char *str)
{
    if (str[0] == c)
        return true;
    // putchar('d');
    return false;
}

list_t *split(char delim, char *str, size_t str_size)
{
    list_t *myvec = list_create();
    string_t *bld = string_create();

    for (size_t i = 0; i < str_size; i++)
    {
        if (str[i] == delim)
        {
            string_append(bld, '\0');
            list_pushback(myvec, bld->chars);

            string_destroy(bld);
            bld = string_create();

            continue;
        }

        string_append(bld, str[i]);
    }
    string_append(bld, '\0');
    list_pushback(myvec, bld->chars);
    string_destroy(bld);
    return myvec;
}

list_t *split_lim(char delim, char *str, size_t str_size, int lim)
{
    list_t *myvec = list_create();
    string_t *bld = string_create();

    bool found = false;
    int times_found = 1;

    for (size_t i = 0; i < str_size; i++)
    {

        if ((str[i] == delim && !found) && times_found < lim)
        {
            string_append(bld, '\0');
            list_pushback(myvec, bld->chars);
            string_destroy(bld);
            bld = string_create();
            times_found++;
            if (times_found == lim)
                found = true;
            continue;
        }

        string_append(bld, str[i]);
    }

    string_append(bld, '\0');
    list_pushback(myvec, bld->chars);
    string_destroy(bld);
    return myvec;
}

char *setchar_at(char *str, char c, size_t pos, size_t len)
{
    if (pos >= len)
        return str;

    string_t *temp = string_create();
    for (size_t i = 0; i < len; i++)
    {
        if (i == pos)
        {
            string_append(temp, c);
            continue;
        }

        string_append(temp, str[i]);
    }
    string_append(temp, '\0');
    char *chars = temp->chars;
    free(temp);
    return chars;
}

char *remove_leading_and_trailing_spaces(char *buffer)
{
    int start = find_first_not_space(buffer);
    int stop = find_last_not_space(buffer);

    return string_sub(start,stop,buffer,strlen(buffer));
}

size_t find_last_not_space(char *str)
{
    size_t len = strlen(str);
    size_t i;

    for(i = len-1; i ; i--)
    {
        if(str[i] != ' ')
        {
            if(str[i] != '\t')
                break;
        }
    }

    return i;
}

size_t find_first_not_space(char *str)
{
    size_t len = strlen(str);
    size_t i;

    for(i = 0; i < len ; i++)
    {
        if(str[i] != ' ')
        {
            if(str[i] != '\t')
                break;
        }
    }

    return i;
}


char *string_to_lower(char *str)
{
    string_t *low = string_create();
    size_t len = strlen(str);

    for(size_t i = 0; i < len; i++)
    {
        if(str[i] >= 'A' && str[i] <= 'Z')
        {
            string_append(low,str[i] + 32);
            continue;
        }

        string_append(low,str[i]);
    }

    string_append(low, '\0');
    char *chars = low->chars;
    free(low);
    return chars;
}


int uuid_v4_gen(char *buffer)
{
	union
	{
		struct
		{
			uint32_t time_low;
			uint16_t time_mid;
			uint16_t time_hi_and_version;
			uint8_t  clk_seq_hi_res;
			uint8_t  clk_seq_low;
			uint8_t  node[6];
		};
		uint8_t __rnd[16];
	} uuid;

	int rc = RAND_bytes(uuid.__rnd, sizeof(uuid));

	uuid.clk_seq_hi_res = (uint8_t) ((uuid.clk_seq_hi_res & 0x3F) | 0x80);
	uuid.time_hi_and_version = (uint16_t) ((uuid.time_hi_and_version & 0x0FFF) | 0x4000);

	snprintf(buffer, 38, "%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
			uuid.time_low, uuid.time_mid, uuid.time_hi_and_version,
			uuid.clk_seq_hi_res, uuid.clk_seq_low,
			uuid.node[0], uuid.node[1], uuid.node[2],
			uuid.node[3], uuid.node[4], uuid.node[5]);

	return rc;
}


unsigned char *string_encrypt(char *instr,char *key_path)
{
    FILE *f = fopen(key_path,"r");

    if(f == NULL)
        return NULL;

    EVP_PKEY *pkey = PEM_read_PUBKEY(f,NULL,NULL,NULL);

    if(!pkey)
        return NULL;

    fclose(f);

    EVP_PKEY_CTX *ctx;

    ctx = EVP_PKEY_CTX_new(pkey,NULL);

    if(!ctx)
        return NULL;

    EVP_PKEY_encrypt_init(ctx);

    size_t cipher_txt_len;

    if(EVP_PKEY_encrypt(ctx,NULL,&cipher_txt_len,(const unsigned char *)instr,strlen(instr)) <= 0)
        return NULL;

    unsigned char *out = OPENSSL_malloc(cipher_txt_len);

    if(out == NULL)
        return out;

    if(EVP_PKEY_encrypt(ctx,out,&cipher_txt_len,(const unsigned char *)instr,strlen(instr)) <= 0)
    {
        OPENSSL_free(out);
        return NULL;
    }

    unsigned char *res = base64_encode(out,cipher_txt_len);
    
    OPENSSL_free(out);

    return res;
}

