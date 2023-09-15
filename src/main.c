#include <stdio.h>
#include "./include/string_buffer.h"
#include "./include/list.h"
#include "./include/map.h"
#include "./include/lib.h"
#include "./include/requests.h"
#include "./include/dbc.h"
#include <json-c/json.h>
#include <time.h>

int main()
{
    time_t rawtime;
    struct tm * timeinfo;

    time( &rawtime );

    timeinfo = localtime( &rawtime );

    printf("%u\n",(unsigned) time(NULL));
}
