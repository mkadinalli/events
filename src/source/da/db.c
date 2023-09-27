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
    conn = create_connection_from_a_file("/home/vic/Desktop/ev2/events/config/config.json");

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
    conn = create_connection_from_a_file("/home/vic/Desktop/ev2/events/config/config.json");

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


conn_pool * create_conn_pool(size_t size)
{
    size_t checked_size = 2;

    if(size > 2)
    {
        checked_size = size;
    }

    if(checked_size > 10)
    {
        puts("too large");
        exit(1);
    }

    db_conn * conns = malloc(sizeof(db_conn));
    conns->connection = create_connection_from_a_file("/home/vic/Desktop/ev2/events/config/config.json");
    conns->busy = false;
    conns->next = NULL;
    conns->t_id = conns->connection->thread_id;

    for(size_t i = 1; i < checked_size; i++)
    {
        db_conn *temp = malloc(sizeof(db_conn));
        temp->connection = create_connection_from_a_file("/home/vic/Desktop/ev2/events/config/config.json");
        temp->next = conns;
        temp->busy = false;
        temp->t_id = temp->connection->thread_id;
        mtx_init(&(temp->dbmtx),0);

        conns = temp;
    }

    conn_pool * cpool = malloc(sizeof(conn_pool));

    cpool->connections = conns;
    cpool->busy_conns = 0;
    cnd_init(&(cpool->conn_condition));
    mtx_init(&(cpool->conn_mtx),0);
    cpool->pool_size = checked_size;

    return cpool;
}



MYSQL *cpool_get_connection(conn_pool *cpool)
{
    if(cpool == NULL) return NULL;

    MYSQL * conn = NULL;

    db_conn * d = cpool->connections;

    mtx_lock(&(cpool->conn_mtx));
    while(cpool->busy_conns >= cpool->pool_size)
        cnd_wait(&(cpool->conn_condition),&(cpool->conn_mtx));

    while(d)
    {
        if(!d->busy)
        {
            conn = d->connection;
            d->busy = true;
            cpool->busy_conns++;
            mtx_unlock(&(cpool->conn_mtx));
            break;
        }
        d=d->next;
    }

    if(conn == NULL){

     mtx_unlock(&(cpool->conn_mtx));

     puts("----returning a null connection-----");

     printf("Busy connections -> %ld\n",cpool->busy_conns);

    }

    return conn;
}


bool cpool_drop_connection(MYSQL *con ,conn_pool *cpool)
{
    if(cpool == NULL) return false;

    db_conn * d = cpool->connections;

    bool out = false;

    while(d)
    {
        if(d->t_id == con->thread_id)
        {
            out = true;
            mtx_lock(&(cpool->conn_mtx));
            cpool->busy_conns--;
            //d->t_id = con->thread_id;
            mtx_unlock(&(cpool->conn_mtx));
            //con = mysql_init(con);
            
            d->busy = false;
            cnd_signal(&(cpool->conn_condition));
            break;

        }

        d=d->next;
    }

    return out;
}

void cpool_destroy(conn_pool *cpool)
{
    if(cpool == NULL) return;


    db_conn * d = cpool->connections;

    while(d)
    {
        db_conn * f = d;
        d=d->next;
        mysql_close(f->connection);
        free(f);
    }
    
    cnd_destroy(&(cpool->conn_condition));
    mtx_destroy(&(cpool->conn_mtx));
    free(cpool);
}
