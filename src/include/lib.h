#pragma once
#include "./string_buffer.h"
#include "./list.h"
#include "./map.h"
#include <mysql/mysql.h>
#include <json-c/json.h>

char * string_removechar(char c, char *buffer, size_t len);
char *string_replacechar(char previous, char current, char *buffer, size_t len);
char *string_removechar_at(unsigned int pos, char *buffer, size_t len);
int string_last_index_of(char c, char *str, unsigned int len);
char *string_sub(unsigned int start, unsigned int stop, char *buffer, unsigned int len);
bool endswith(char c, char *str);
bool startswith(char c,char *str);
list_t *split(char delim, char *str, size_t str_size);
list_t *split_lim(char delim, char *str, size_t str_size, int lim);
char* setchar_at(char *str,char c,size_t pos,size_t len);
bool starts_with_word(char *word,char *str);

//===========================================================file

char * read_file_to_string(char *path);
MYSQL * create_connection_from_a_file(MYSQL ** sql_struct,char *path_to_config);

//==========================================================http;
