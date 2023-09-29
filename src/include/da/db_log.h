#pragma once

#include <stdio.h>
#include <string.h>
#include "files.h"

void
write_mysql_error_to_file(const char *sql_state,const char *error,char *f,int line);