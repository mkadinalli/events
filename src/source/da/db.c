#include "../../include/da/db.h"
#include "../../include/lib/files.h"

#include <string.h>


bool check_if_user_data_exists(const char *username, const char *email)
{
    char *queryfmt = "select * from users where email = '%s' or username = '%s'";

    char query[100];

    sprintf(query, queryfmt, email, username);

    int r = find_row_count(query);

    if (r > 0)
        return true;
    return false;
}

bool check_if_user_exists(char *identity, char *password, bool by_email)
{
    char *queryfmt = "call validate_user( %s ,%s ,%s)";

    char *identity_c = string_cover(identity);
    char *password_c = string_cover(password);

    char query[100];

    sprintf(query, queryfmt, password_c, by_email ? identity_c : "null", by_email ? "null" : identity_c);

    free(identity_c);
    free(password_c);

    puts(query);

    if (find_row_count(query) == -1)
        return false;
    return true;
}

int find_row_count(char *query)
{
    MYSQL *conn = NULL;
    conn = mysql_init(conn);
    conn = create_connection_from_a_file(conn,
                                         "/home/vic/Desktop/ev2/events/config/config.json");

    if (conn == NULL)
    {
        puts("failed to connect to db");
        return -1;
    }

    if (mysql_query(conn, query))
    {
        return -1;
    }

    MYSQL_RES *result = NULL;
    result = mysql_store_result(conn);

    if (!result)
        return -1;

    int rows = mysql_num_rows(result);

    mysql_free_result(result);
    mysql_close(conn);

    return rows;
}

bool execute_query(char *query)
{
    MYSQL *conn = NULL;
    conn = mysql_init(conn);
    conn = create_connection_from_a_file(conn,
                                         "/home/vic/Desktop/ev2/events/config/config.json");

    if (conn == NULL)
    {
        puts("failed to connect to db");
        return false;
    }

    if (mysql_query(conn, query))
    {
        return false;
    }

    mysql_close(conn);

    return true;
}
