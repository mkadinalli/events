#pragma once

#include <mysql/mysql.h>
#include <json-c/json.h>

#include "../data-structures/ev_string.h"
#include "../data-structures/list.h"
#include "../data-structures/list.h"


char *
string_removechar(char c,
                  char *buffer,
                  size_t len);
char *
string_replacechar(char previous,
                   char current,
                   char *buffer,
                   size_t len);
char *
string_removechar_at(unsigned int pos,
                     char *buffer,
                     size_t len);
int string_last_index_of(char c,
                         char *str,
                         unsigned int len);
char *
string_sub(unsigned int start,
           unsigned int stop,
           char *buffer,
           unsigned int len);
bool endswith(char c,
              char *str);
bool startswith(char c,
                char *str);
struct
    list_t *
    split(char delim,
          char *str,
          size_t str_size);
struct
    list_t *
    split_lim(char delim,
              char *str,
              size_t str_size,
              int lim);
char *
setchar_at(char *str,
           char c,
           size_t pos,
           size_t len);
bool starts_with_word(char *word,
                      char *str);
char *
string_create_copy(char *str);

char *string_cover(char *str);

char *base64_encode(const char *str ,size_t len);

char *remove_leading_and_trailing_spaces(char *str);

size_t find_last_not_space(char *str);

size_t find_first_not_space(char *str);
