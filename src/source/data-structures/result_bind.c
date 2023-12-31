#include "result_bind.h"

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


void result_bind_get_string_ex(size_t pos, result_bind *rb,char *str)
{
    
    result_bind *tmp = result_bind_get_at(pos, rb);

    assert(tmp != NULL);
    assert((int)pos < result_bind_get_size(rb));

    strcpy(str,tmp->value);
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
    while (true)
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

        if((int)pos == result_bind_get_size(rb))
            break;
        tmp = tmp->next;
    }
}

void result_bind_set_int(size_t pos, result_bind *rb,int value)
{
    assert(rb != NULL);
    assert((int)pos < result_bind_get_size(rb));
    assert(sizeof(value) == sizeof(int));

    result_bind_realloc(pos,0,MYSQL_TYPE_SHORT,rb);

    char tmp[20];

    sprintf(tmp,"%d",value);

    strcpy(result_bind_get_at(pos,rb)->value,tmp);
}

void result_bind_set_double(size_t pos, result_bind *rb,double value)
{
    assert(rb != NULL);
    assert((int)pos < result_bind_get_size(rb));
    assert(sizeof(value) == sizeof(double));

    result_bind_realloc(pos,0,MYSQL_TYPE_DOUBLE,rb);

    char tmp[30];

    sprintf(tmp,"%f",value);

    strcpy(result_bind_get_at(pos,rb)->value,tmp);
}

void result_bind_set_float(size_t pos, result_bind *rb,float value)
{
    assert(rb != NULL);
    assert((int)pos < result_bind_get_size(rb));
    assert(sizeof(value) == sizeof(float));

    result_bind_realloc(pos,0,MYSQL_TYPE_FLOAT,rb);

    char tmp[30];

    sprintf(tmp,"%f",value);

    strcpy(result_bind_get_at(pos,rb)->value,tmp);
}

void result_bind_set_string(size_t pos, result_bind *rb,const char *value)
{
    assert(rb != NULL);
    assert((int)pos < result_bind_get_size(rb));

    result_bind_realloc(pos,strlen(value) +1,MYSQL_TYPE_STRING,rb);

    char tmp[strlen(value)+1];

    sprintf(tmp,"%s",value);

    strcpy(result_bind_get_at(pos,rb)->value,tmp);
}

void  result_bind_set_bool(size_t pos, result_bind *rb,bool value)
{
    assert(rb != NULL);
    assert((int)pos < result_bind_get_size(rb));
    assert(value == true || value == false);
    assert(sizeof(value) == sizeof(bool));

    result_bind_realloc(pos,0,MYSQL_TYPE_BOOL,rb);

    char tmp[2];

    sprintf(tmp,"%d",value);

    strcpy(result_bind_get_at(pos,rb)->value,tmp);
}

void result_bind_set_i32(size_t pos, result_bind *rb,long value)
{
    assert(rb != NULL);
    assert((int)pos < result_bind_get_size(rb));
    assert(sizeof(value) == sizeof(long));

    result_bind_realloc(pos,0,MYSQL_TYPE_LONG,rb);

    char tmp[40];

    sprintf(tmp,"%ld",value);

    strcpy(result_bind_get_at(pos,rb)->value,tmp);
}

void result_bind_set_i64(size_t pos, result_bind *rb,long long value)
{
    assert(rb != NULL);
    assert((int)pos < result_bind_get_size(rb));
    assert(sizeof(value) == sizeof(long long));

    result_bind_realloc(pos,0,MYSQL_TYPE_LONGLONG,rb);

    char tmp[40];

    sprintf(tmp,"%lld",value);

    strcpy(result_bind_get_at(pos,rb)->value,tmp);
}

void result_bind_set_u32(size_t pos, result_bind *rb,unsigned long value)
{
    assert(rb != NULL);
    assert((int)pos < result_bind_get_size(rb));
    assert(sizeof(value) == sizeof(long));

    result_bind_realloc(pos,0,MYSQL_TYPE_LONG,rb);

    char tmp[40];

    sprintf(tmp,"%ld",value);

    strcpy(result_bind_get_at(pos,rb)->value,tmp);
}

void result_bind_set_u64(size_t pos, result_bind *rb,unsigned long long value)
{
    assert(rb != NULL);
    assert((int)pos < result_bind_get_size(rb));
    assert(sizeof(value) == sizeof(long long));

    result_bind_realloc(pos,0,MYSQL_TYPE_LONGLONG,rb);

    char tmp[40];

    sprintf(tmp,"%lld",value);

    strcpy(result_bind_get_at(pos,rb)->value,tmp);
}