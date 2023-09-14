#include <stdio.h>
#include "./include/string_buffer.h"
#include "./include/list.h"
#include "./include/map.h"
#include "./include/lib.h"
#include "./include/requests.h"
#include "./include/dbc.h"
#include <json-c/json.h>

int main()
{
  MYSQL * sql = NULL;
  sql = mysql_init(sql);

  if((create_connection_from_a_file(&sql,"/home/vic/Desktop/ev2/events/config/config.json")) == NULL)
  {
    puts("connection failed");
    mysql_close(sql);
    exit(EXIT_FAILURE);
  }

  mysql_close(sql);
}
