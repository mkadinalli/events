#pragma once
#include "./string_buffer.h"
#include "./list.h"
#include "./map.h"

char *string_removechar(char c, char *buffer, size_t len);
char *string_replacechar(char previous, char current, char *buffer, size_t len);
char *string_removechar_at(unsigned int pos, char *buffer, size_t len);
int string_last_index_of(char c, char *str, unsigned int len);
char *string_sub(unsigned int start, unsigned int stop, char *buffer, unsigned int len);
bool endswith(char c, char *str);
list_t *split(char delim, char *str, size_t str_size);
list_t *split_lim(char delim, char *str, size_t str_size, int lim);
