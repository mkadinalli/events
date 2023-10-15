#include "ev_time.h"

void get_current_time(char *current_time)
{
   struct tm * t = NULL;
    time_t time_;

    time_ = time(&time_);

    t = localtime(&time_);

    strftime(current_time,100,"%Y%m%d%H%M%S",t);

}