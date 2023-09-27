#include <stdio.h>
#include <json-c/json.h>
#include <assert.h>
#include "./include/http/server.h"
#include <mysql/mysql.h>
#include "lib/files.h"
#include <stdbool.h>
#include "da/db.h"

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
    if(rb == NULL) return;
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


void eexecute_query(char *query,MYSQL *conn)
{
    MYSQL_STMT *stmt = mysql_stmt_init(conn);

    assert(stmt != NULL);

    //assert(!mysql_stmt_close(stmt));

    int status;

    status = mysql_stmt_prepare(stmt,query,strlen(query));

    assert(status == 0);

    status = mysql_stmt_execute(stmt);

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
    memset(result_outputs, 0, sizeof result_outputs);
    unsigned long lengths[col_count];
    bool is_null[col_count];
    bool error[col_count];

    result_bind * bnd = result_bind_create(col_count);

    for(unsigned int i = 0; i < col_count; i++)
    {
        result_outputs[i].buffer_type = MYSQL_TYPE_STRING;
        result_outputs[i].is_null = &(is_null[i]);
        result_outputs[i].length = &(lengths[i]);
        result_outputs[i].error = &(error[i]);

        result_bind_realloc(i,columns[i].length,columns[i].type,bnd);

        result_outputs[i].buffer = result_bind_get_at(i,bnd)->value;
        result_outputs[i].buffer_length = 1000;
        //strlen(result_bind_get_at(i,bnd)->value);
    }

    if (mysql_stmt_bind_result(stmt, result_outputs))
    {
        puts("================bind failed===================");
        exit(1);
    }

    while(!mysql_stmt_fetch(stmt))
    {
        puts("===========================");
        //result_bind_print(bnd);
        puts(result_bind_get_string(0,bnd));
        puts(result_bind_get_string(1,bnd));
        puts(result_bind_get_string(2,bnd));
        puts(result_bind_get_string(3,bnd));
        puts("===========================");
    }

    printf("fetched -> %ld rows.\n",mysql_num_rows(result_meta_data));

    result_bind_destroy(bnd);
    mysql_free_result(result_meta_data);
    assert(!mysql_stmt_close(stmt));
}

int main()
{
    conn_pool *cp = create_conn_pool(5);

    for(int i = 0; i < 10;i++)
    {

    MYSQL *c = cpool_get_connection(cp);

    assert(c != NULL);

    eexecute_query("select cast(bin_to_uuid(id) as char) as id ,name,username,email from users",c);

        if(cpool_drop_connection(c,cp))
            puts("Connection dropped succesfully");
        else
            puts("Connection drop failed");
    }

}
