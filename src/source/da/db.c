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

    if (find_row_count(query) == -1)
        return false;
    return true;
}

int find_row_count(char *query)
{

    MYSQL *conn = cpool_get_connection(cpool);

    if (conn == NULL)
    {
        //log
        abort();
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
    cpool_drop_connection(conn,cpool);

    return rows;
}

json_object * execute_prepared_statement(MYSQL_STMT *stmt)
{
    assert(stmt != NULL);

    int status;

    status = mysql_stmt_execute(stmt);

    assert(status == 0);

    MYSQL_RES *result_meta_data = mysql_stmt_result_metadata(stmt);

    assert(result_meta_data != NULL);

    unsigned int col_count = mysql_num_fields(result_meta_data);
    MYSQL_FIELD *columns = mysql_fetch_fields(result_meta_data);

    assert(columns != NULL);

    MYSQL_BIND result_outputs[col_count];
    memset(result_outputs, 0, sizeof result_outputs);
    unsigned long lengths[col_count];
    bool is_null[col_count];
    bool error[col_count];

    result_bind *bnd = result_bind_create(col_count);

    for (unsigned int i = 0; i < col_count; i++)
    {
        result_outputs[i].buffer_type = MYSQL_TYPE_STRING;
        result_outputs[i].is_null = &(is_null[i]);
        result_outputs[i].length = &(lengths[i]);
        result_outputs[i].error = &(error[i]);

        result_bind_realloc(i, columns[i].length, columns[i].type, bnd);

        result_outputs[i].buffer = result_bind_get_at(i, bnd)->value;
        result_outputs[i].buffer_length = 1000;
        strlen(result_bind_get_at(i, bnd)->value);
    }

    if (mysql_stmt_bind_result(stmt, result_outputs))
    {
        //log
        abort();
    }

    json_object *res = json_object_new_array();

    while (!mysql_stmt_fetch(stmt))
    {
        json_object *this_row = json_object_new_object();

        for (size_t i = 0; i < col_count; i++)
        {
            json_object_object_add(this_row, columns[i].name,
                                   json_object_new_string(result_bind_get_at(i, bnd)->value));
        }

        json_object_array_add(res, json_object_get(this_row));

        json_object_put(this_row);
    }

    mysql_free_result(result_meta_data);

    return res;
}

json_object *execute_prepared_query(char *query, result_bind *params)
{
    assert(query != NULL);

    MYSQL *conn = cpool_get_connection(cpool);

    MYSQL_STMT *stmt = NULL;
    stmt = mysql_stmt_init(conn);

    assert(stmt != NULL);

    if(mysql_stmt_prepare(stmt, query, strlen(query)))
    {
       //log

        abort();
    }


    if (params != NULL)
    {
        int param_count = mysql_stmt_param_count(stmt);

        assert(param_count == result_bind_get_size(params));

        MYSQL_BIND p_bind[param_count];
        memset(p_bind, 0, sizeof p_bind);

        result_bind_print(params);

        unsigned long len[param_count];

        for (int i = 0; i < param_count; i++)
        {       
            len[i] = strlen(result_bind_get_string(i, params));

            p_bind[i].buffer_type = result_bind_get_at(i, params)->type_name;
            p_bind[i].is_null = 0;
            p_bind[i].length = &(len[i]);
            p_bind[i].buffer = result_bind_get_string(i, params);
            p_bind[i].buffer_length = 100;

        }

        bool status = mysql_stmt_bind_param(stmt, p_bind);

        assert(!status);
    }

    assert(stmt != NULL);

    int status;

    status = mysql_stmt_execute(stmt);

    if(status != 0)
    {
        puts(mysql_stmt_error(stmt));
    }

    assert(status == 0);

    MYSQL_RES *result_meta_data = mysql_stmt_result_metadata(stmt);

    assert(result_meta_data != NULL);

    unsigned int col_count = mysql_num_fields(result_meta_data);
    MYSQL_FIELD *columns = mysql_fetch_fields(result_meta_data);

    assert(columns != NULL);

    MYSQL_BIND result_outputs[col_count];
    memset(result_outputs, 0, sizeof result_outputs);
    unsigned long lengths[col_count];
    bool is_null[col_count];
    bool error[col_count];

    result_bind *bnd = result_bind_create(col_count);

    for (unsigned int i = 0; i < col_count; i++)
    {
        result_outputs[i].buffer_type = MYSQL_TYPE_STRING;
        result_outputs[i].is_null = &(is_null[i]);
        result_outputs[i].length = &(lengths[i]);
        result_outputs[i].error = &(error[i]);

        result_bind_realloc(i, columns[i].length, columns[i].type, bnd);

        result_outputs[i].buffer = result_bind_get_at(i, bnd)->value;
        result_outputs[i].buffer_length = 1000;
        strlen(result_bind_get_at(i, bnd)->value);
    }

    if (mysql_stmt_bind_result(stmt, result_outputs))
    {
        //log
        abort();
    }

    json_object *res = json_object_new_array();

    while (!mysql_stmt_fetch(stmt))
    {
        json_object *this_row = json_object_new_object();

        for (size_t i = 0; i < col_count; i++)
        {
            json_object_object_add(this_row, columns[i].name,
                                   json_object_new_string(result_bind_get_at(i, bnd)->value));
        }

        json_object_array_add(res, json_object_get(this_row));

        json_object_put(this_row);
    }

    result_bind_destroy(bnd);
    mysql_free_result(result_meta_data);

    cpool_drop_connection(conn,cpool);

    return res;
}


json_object *execute_prepared_call_query(char *query, result_bind *params)
{
    assert(query != NULL);

    MYSQL *conn = cpool_get_connection(cpool);

    MYSQL_STMT *stmt = NULL;
    stmt = mysql_stmt_init(conn);

    assert(stmt != NULL);

    if(mysql_stmt_prepare(stmt, query, strlen(query)))
    {
        //log

        abort();
    }


    if (params != NULL)
    {
        int param_count = mysql_stmt_param_count(stmt);

        assert(param_count == result_bind_get_size(params));

        MYSQL_BIND p_bind[param_count];
        memset(p_bind, 0, sizeof p_bind);

        unsigned long len[param_count];

        for (int i = 0; i < param_count; i++)
        {       
            len[i] = strlen(result_bind_get_string(i, params));

            p_bind[i].buffer_type = result_bind_get_at(i, params)->type_name;
            p_bind[i].is_null = 0;
            p_bind[i].length = &(len[i]);
            p_bind[i].buffer = result_bind_get_string(i, params);
            p_bind[i].buffer_length = 100;
        }

        bool status = mysql_stmt_bind_param(stmt, p_bind);

        assert(!status);
    }

    assert(stmt != NULL);

    int status;

    status = mysql_stmt_execute(stmt);

    if(status != 0)
    {
        puts(mysql_stmt_error(stmt));
    }

    assert(status == 0);

    MYSQL_RES *result_meta_data = mysql_stmt_result_metadata(stmt);

    assert(result_meta_data != NULL);

    unsigned int col_count = mysql_num_fields(result_meta_data);
    MYSQL_FIELD *columns = mysql_fetch_fields(result_meta_data);

    assert(columns != NULL);


    MYSQL_BIND result_outputs[col_count];
    memset(result_outputs, 0, sizeof result_outputs);
    unsigned long lengths[col_count];
    bool is_null[col_count];
    bool error[col_count];

    result_bind *bnd = result_bind_create(col_count);

    for (unsigned int i = 0; i < col_count; i++)
    {
        result_outputs[i].buffer_type = MYSQL_TYPE_STRING;
        result_outputs[i].is_null = &(is_null[i]);
        result_outputs[i].length = &(lengths[i]);
        result_outputs[i].error = &(error[i]);

        result_bind_realloc(i, columns[i].length, columns[i].type, bnd);

        result_outputs[i].buffer = result_bind_get_at(i, bnd)->value;
        result_outputs[i].buffer_length = 1000;
        strlen(result_bind_get_at(i, bnd)->value);
    }

    if (mysql_stmt_bind_result(stmt, result_outputs))
    {
        abort();
    }

    json_object *res = json_object_new_array();

    while (!mysql_stmt_fetch(stmt))
    {
        json_object *this_row = json_object_new_object();

        for (size_t i = 0; i < col_count; i++)
        {
            json_object_object_add(this_row, columns[i].name,
                                   json_object_new_string(result_bind_get_at(i, bnd)->value));
        }

        json_object_array_add(res, json_object_get(this_row));

        json_object_put(this_row);
    }

    while(mysql_stmt_next_result(stmt) == 0){}

    result_bind_destroy(bnd);
    mysql_free_result(result_meta_data);
    assert(!mysql_stmt_close(stmt));

    cpool_drop_connection(conn,cpool);

    return res;
}

conn_pool *create_conn_pool(size_t size)
{
    size_t checked_size = 2;

    if (size > 2)
    {
        checked_size = size;
    }

    if (checked_size > 10)
    {
        puts("too large");
        exit(1);
    }

    db_conn *conns = malloc(sizeof(db_conn));
    conns->connection = create_connection_from_a_file("/home/vic/Desktop/ev2/events/config/config.json");
    conns->busy = false;
    conns->next = NULL;
    conns->t_id = conns->connection->thread_id;

    for (size_t i = 1; i < checked_size; i++)
    {
        db_conn *temp = malloc(sizeof(db_conn));
        temp->connection = create_connection_from_a_file("/home/vic/Desktop/ev2/events/config/config.json");
        temp->next = conns;
        temp->busy = false;
        temp->t_id = temp->connection->thread_id;
        mtx_init(&(temp->dbmtx), 0);

        conns = temp;
    }

    conn_pool *cpool = malloc(sizeof(conn_pool));

    cpool->connections = conns;
    cpool->busy_conns = 0;
    cnd_init(&(cpool->conn_condition));
    mtx_init(&(cpool->conn_mtx), 0);
    cpool->pool_size = checked_size;

    return cpool;
}

MYSQL *cpool_get_connection(conn_pool *cpool)
{
    if (cpool == NULL)
        return NULL;

    MYSQL *conn = NULL;

    db_conn *d = cpool->connections;

    mtx_lock(&(cpool->conn_mtx));
    while (cpool->busy_conns >= cpool->pool_size)
        cnd_wait(&(cpool->conn_condition), &(cpool->conn_mtx));

    while (d)
    {
        if (!d->busy)
        {
            conn = d->connection;
            d->busy = true;
            cpool->busy_conns++;
            mtx_unlock(&(cpool->conn_mtx));
            break;
        }
        d = d->next;
    }

    if (conn == NULL)
    {

        mtx_unlock(&(cpool->conn_mtx));

        puts("----returning a null connection-----");

        printf("Busy connections -> %ld\n", cpool->busy_conns);
    }

    return conn;
}

bool cpool_drop_connection(MYSQL *con, conn_pool *cpool)
{
    if (cpool == NULL)
        return false;

    db_conn *d = cpool->connections;

    bool out = false;

    while (d)
    {
        if (d->t_id == con->thread_id)
        {
            out = true;
            mtx_lock(&(cpool->conn_mtx));
            cpool->busy_conns--;
            // d->t_id = con->thread_id;
            mtx_unlock(&(cpool->conn_mtx));
            // con = mysql_init(con);

            d->busy = false;
            cnd_signal(&(cpool->conn_condition));
            break;
        }

        d = d->next;
    }

    return out;
}

void cpool_destroy(conn_pool *cpool)
{
    if (cpool == NULL)
        return;

    db_conn *d = cpool->connections;

    while (d)
    {
        db_conn *f = d;
        d = d->next;
        mysql_close(f->connection);
        free(f);
    }

    cnd_destroy(&(cpool->conn_condition));
    mtx_destroy(&(cpool->conn_mtx));
    free(cpool);
}

/*

void eexecute_query(char *query,MYSQL *conn)
{
    MYSQL_STMT *stmt = mysql_stmt_init(conn);

    assert(stmt != NULL);

    //assert(!mysql_stmt_close(stmt));

    int status;

    status = mysql_stmt_prepare(stmt,query,strlen(query));

    assert(status == 0);

    MYSQL_RES *result_meta_data = mysql_stmt_result_metadata(stmt);

    assert(result_meta_data != NULL);

    unsigned int col_count = mysql_num_fields(result_meta_data);
    MYSQL_FIELD *columns = mysql_fetch_fields(result_meta_data);

    assert(columns != NULL);

    for(unsigned int i = 0; i < col_count;i++)
    {
        puts(columns[i].name);
    }


    MYSQL_BIND result_outputs[col_count];
    unsigned long lengths[col_count];
    bool is_null[col_count];
    bool error[col_count];

    for(unsigned int i = 0; i < col_count; i++)
    {
        result_outputs[i].buffer_type = columns[i].type;
        result_outputs[i].is_null = &(is_null[i]);
        result_outputs[i].length = &(lengths[i]);
        result_outputs[i].error = &(error[i]);
    }

    mysql_free_result(result_meta_data);
    assert(!mysql_stmt_close(stmt));
}





int main()
{
    conn_pool *cp = create_conn_pool(5);

    for(int i = 0; i < 100;i++)
    {

    MYSQL *c = cpool_get_connection(cp);

    assert(c != NULL);

    eexecute_query("select * from users",c);

        if(cpool_drop_connection(c,cp))
            puts("Connection dropped succesfully");
        else
            puts("Connection drop failed");
    }

}

*/

//==========================================================
/*

typedef struct result_bind result_bind;
struct result_bind
{
    int type_name;
    char *value;
    result_bind *next;
};

result_bind *result_bind_create(size_t size)
{
    result_bind *rb = malloc(sizeof(result_bind));
    rb->type_name = -1;
    rb->value = NULL;
    rb->next = NULL;

    for (size_t i = 1; i < size; i++)
    {
        result_bind *tmp = malloc(sizeof(result_bind));
        tmp->type_name = -1;
        tmp->value = NULL;

        tmp->next = rb;

        rb = tmp;
    }

    return rb;
}

int result_bind_get_size(result_bind *rb)
{
    if (!rb)
        return -1;

    result_bind *tmp = rb;

    int s = 0;

    while (tmp)
    {
        s++;
        tmp = tmp->next;
    }

    return s;
}

result_bind *result_bind_get_at(size_t pos, result_bind *rb)
{
    if (rb == NULL)
        return NULL;

    result_bind *tmp = rb;

    size_t s = 0;

    while (tmp)
    {
        if (s == pos)
            break;
        s++;
        tmp = tmp->next;
    }

    return tmp;
}

bool result_bind_realloc(size_t pos, size_t new_size, int type, result_bind *rb)
{
    if (rb)
    {
        result_bind *tmp = result_bind_get_at(pos, rb);
        if (tmp == NULL)
            return false;

        switch (type)
        {
        case MYSQL_TYPE_SHORT:
            tmp->type_name = type;
            tmp->value = malloc(sizeof(int) + 1);
            bzero(tmp->value, sizeof(int) + 1);
            break;

        case MYSQL_TYPE_LONG:
            tmp->type_name = type;
            tmp->value = malloc(sizeof(long) + 1);
            bzero(tmp->value, sizeof(long) + 1);
            break;

        case MYSQL_TYPE_LONGLONG:

            tmp->type_name = type;
            tmp->value = malloc(sizeof(long long) + 1);
            bzero(tmp->value, sizeof(long long) + 1);
            break;

        case MYSQL_TYPE_BOOL:
            tmp->type_name = type;
            tmp->value = malloc(sizeof(bool) + 1);
            bzero(tmp->value, sizeof(bool) + 1);
            break;

        case MYSQL_TYPE_FLOAT:
            tmp->type_name = type;
            tmp->value = malloc(sizeof(float) + 1);
            bzero(tmp->value, sizeof(float) + 1);
            break;

        case MYSQL_TYPE_DOUBLE:
            tmp->type_name = type;
            tmp->value = malloc(sizeof(double) + 1);
            bzero(tmp->value, sizeof(double) + 1);
            break;

        case MYSQL_TYPE_DECIMAL:
            tmp->type_name = type;
            tmp->value = malloc(sizeof(double) + 1);
            bzero(tmp->value, sizeof(double) + 1);
            break;

        default:
            tmp->type_name = MYSQL_TYPE_STRING;
            tmp->value = malloc(new_size + 1);
            bzero(tmp->value, new_size + 1);
            break;
        }

        if (tmp->value)
            return true;
    }

    return false;
}


int result_bind_get_int(size_t pos, result_bind *rb)
{
    result_bind *tmp = result_bind_get_at(pos, rb);

    assert(tmp != NULL);
    assert((int)pos < result_bind_get_size(rb));

    return atoi(tmp->value);
}

double result_bind_get_double(size_t pos, result_bind *rb)
{
    result_bind *tmp = result_bind_get_at(pos, rb);

    assert(tmp != NULL);
    assert((int)pos < result_bind_get_size(rb));

    return strtod(tmp->value, 0);
}

double result_bind_get_float(size_t pos, result_bind *rb)
{
    result_bind *tmp = result_bind_get_at(pos, rb);

    assert(tmp != NULL);
    assert((int)pos < result_bind_get_size(rb));

    return strtof(tmp->value, 0);
}

char *result_bind_get_string(size_t pos, result_bind *rb)
{
    result_bind *tmp = result_bind_get_at(pos, rb);

    assert(tmp != NULL);
    assert((int)pos < result_bind_get_size(rb));

    return tmp->value;
}

bool result_bind_get_bool(size_t pos, result_bind *rb)
{
    result_bind *tmp = result_bind_get_at(pos, rb);

    assert(tmp != NULL);
    assert((int)pos < result_bind_get_size(rb));

    int rt = atoi(tmp->value);

    assert(rt == 1 || rt == 0);

    return rt == 1 ? true : false;
}

long result_bind_get_i32(size_t pos, result_bind *rb)
{
    result_bind *tmp = result_bind_get_at(pos, rb);

    assert(tmp != NULL);
    assert((int)pos < result_bind_get_size(rb));

    return strtol(tmp->value, 0, 0);
}

long long result_bind_get_i64(size_t pos, result_bind *rb)
{
    result_bind *tmp = result_bind_get_at(pos, rb);

    assert(tmp != NULL);
    assert((int)pos < result_bind_get_size(rb));

    return strtoll(tmp->value, 0, 0);
}

unsigned long result_bind_get_u32(size_t pos, result_bind *rb)
{
    result_bind *tmp = result_bind_get_at(pos, rb);

    assert(tmp != NULL);
    assert((int)pos < result_bind_get_size(rb));

    return strtoul(tmp->value, 0, 0);
}

unsigned long long result_bind_get_u64(size_t pos, result_bind *rb)
{
    result_bind *tmp = result_bind_get_at(pos, rb);

    assert(tmp != NULL);
    assert((int)pos < result_bind_get_size(rb));

    return strtoull(tmp->value, 0, 0);
}

void result_bind_print(result_bind *rb)
{
    result_bind *tmp = rb;
    size_t pos = 0;
    while (tmp->value != NULL)
    {
        switch (tmp->type_name)
        {
        case MYSQL_TYPE_SHORT:
            printf("%d\n", result_bind_get_int(pos, rb));
            break;

        case MYSQL_TYPE_LONG:
            printf("%ld\n", result_bind_get_i32(pos, rb));
            break;

        case MYSQL_TYPE_LONGLONG:
            printf("%lld\n", result_bind_get_i64(pos, rb));
            break;

        case MYSQL_TYPE_BOOL:
            printf("%d\n", result_bind_get_bool(pos, rb));
            break;

        case MYSQL_TYPE_FLOAT:
            printf("%f\n", result_bind_get_float(pos, rb));
            break;

        case MYSQL_TYPE_DOUBLE:
            printf("%f\n", result_bind_get_double(pos, rb));
            break;

        case MYSQL_TYPE_DECIMAL:
            printf("%f\n", result_bind_get_double(pos, rb));
            break;

        default:
            printf("%s\n", result_bind_get_string(pos, rb));
            break;
        }

        pos++;
        tmp = tmp->next;
    }
}


void result_bind_destroy(result_bind *rb)
{
    result_bind *tmp = rb;

    while(tmp != NULL)
    {
        result_bind *r = tmp;
        tmp = tmp->next;
        if(r->value != NULL)
            free(r->value);

        free(r);
    }
}

int main()
{
    result_bind *a = result_bind_create(5);

    // assert(result_bind_get_size(a) == 5);
    // assert(result_bind_get_at(0,a) == NULL);

    assert(result_bind_realloc(0, 10, MYSQL_TYPE_STRING, a));
    assert(result_bind_realloc(1, 10, MYSQL_TYPE_FLOAT, a));
    assert(result_bind_realloc(2, 10, MYSQL_TYPE_STRING, a));

    //result_bind_get_at(0, a)->value = "1.90";
    //result_bind_get_at(1, a)->value = "1.90";
    //result_bind_get_at(2, a)->value = "1.90";
    result_bind_print(a);
    result_bind_destroy(a);

}
*/