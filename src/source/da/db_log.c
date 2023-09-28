#include "db_log.h"
#include "ev_time.h"

void
write_mysql_error_to_file(const char *sql_state,const char *error,char *f,int line)
{
    int stl = strlen(sql_state);
    int erl = strlen(error);
    char current_time[100];

    get_current_time(current_time);

    int timel = strlen(current_time);

    char full_error[stl+erl+timel+100];

    sprintf(full_error," [%s, %d]> %s : %s - %s",f,line,sql_state,error,current_time);

    open_file_to_append("/home/vic/Desktop/ev2/events/logs/db.log",full_error);
}