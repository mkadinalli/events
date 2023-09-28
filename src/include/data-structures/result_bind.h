#pragma once

#include <stdio.h>
#include <json-c/json.h>
#include <assert.h>
#include <mysql/mysql.h>
#include <stdbool.h>
//#include "da/db.h"
#include <string.h>
#include "ev_string.h"

typedef struct result_bind result_bind;
struct result_bind
{
    int type_name;
    char *value;
    result_bind *next;
};

result_bind *result_bind_create(size_t size);

int result_bind_get_size(result_bind *rb);

result_bind *result_bind_get_at(size_t pos, result_bind *rb);

bool result_bind_realloc(size_t pos, size_t new_size, int type, result_bind *rb);

int result_bind_get_int(size_t pos, result_bind *rb);

double result_bind_get_double(size_t pos, result_bind *rb);

double result_bind_get_float(size_t pos, result_bind *rb);

char *result_bind_get_string(size_t pos, result_bind *rb);

bool result_bind_get_bool(size_t pos, result_bind *rb);

long result_bind_get_i32(size_t pos, result_bind *rb);

long long result_bind_get_i64(size_t pos, result_bind *rb);

unsigned long result_bind_get_u32(size_t pos, result_bind *rb);

unsigned long long result_bind_get_u64(size_t pos, result_bind *rb);

//sets

void result_bind_set_int(size_t pos, result_bind *rb,int value);

void result_bind_set_double(size_t pos, result_bind *rb,double value);

void result_bind_set_float(size_t pos, result_bind *rb,float value);

void result_bind_set_string(size_t pos, result_bind *rb,const char *value);

void  result_bind_set_bool(size_t pos, result_bind *rb,bool value);

void result_bind_set_i32(size_t pos, result_bind *rb,long value);

void result_bind_set_i64(size_t pos, result_bind *rb,long long value);

void result_bind_set_u32(size_t pos, result_bind *rb,unsigned long value);

void result_bind_set_u64(size_t pos, result_bind *rb,unsigned long long value);

void result_bind_print(result_bind *rb);

void result_bind_destroy(result_bind *rb);

void result_bind_get_string_ex(size_t pos, result_bind *rb,char *str);