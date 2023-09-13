#include "../include/json.h"

bool is_array = false;
char *json_check_syntax(char *j_string)
{
    string_t *json_string = string_create_from_string(j_string);
    string_t *curls = string_create();
    string_t *squares = string_create();
    string_t *symbols = string_create();

    char USED_QUOTATION = ' ';

    if (endswith(SQUARE_CLOSE_BRACKETS, json_string->chars) &&
        startswith(SQUARE_OPEN_BRACKETS, json_string->chars))
    {
        is_array = true;
    }

    if (is_array)
    {
        for (size_t i = 0; i < strlen(json_string->chars); i++)
        {
            if (json_string->chars[i] == SINGLE_QUOTE || json_string->chars[i] == DOUBLE_QUOTE)
            {
                if (json_string->chars[i] == '\'')
                {
                    USED_QUOTATION = '\'';
                }
                else
                {
                    USED_QUOTATION = '"';
                }
                break;
            }
        }

        if (!(endswith(SQUARE_CLOSE_BRACKETS, json_string->chars)) ||
            !(startswith(SQUARE_OPEN_BRACKETS, json_string->chars)))
        {
            puts("Unsupported File Format");
            return NULL;
        }

        if (USED_QUOTATION == ' ')
        {
            puts("Unsupported File Format");
            return NULL;
        }
    }

    else

    {
        for (size_t i = 0; i < strlen(json_string->chars); i++)
        {
            if (json_string->chars[i] == SINGLE_QUOTE || json_string->chars[i] == DOUBLE_QUOTE)
            {
                if (json_string->chars[i] == '\'')
                {
                    USED_QUOTATION = '\'';
                }
                else
                {
                    USED_QUOTATION = '"';
                }
                break;
            }
        }

        if (!(endswith(CURLY_CLOSE_BRACKETS, json_string->chars)) ||
            !(startswith(CURLY_OPEN_BRACKETS, json_string->chars)))
        {
            puts("Unsupported File Format");
            return NULL;
        }

        if (USED_QUOTATION == ' ')
        {
            puts("Unsupported File Format");
            return NULL;
        }
    }

    if (strlen(curls->chars) > 0 || strlen(squares->chars) > 0)
    {
        puts("syntax error");
        printf("%s -> %s\n", curls->chars, squares->chars);
        return NULL;
    }

    string_destroy(symbols);
    string_destroy(curls);
    string_destroy(squares);

    for (size_t i = 0; i < strlen(json_string->chars); i++)
    {
        if (json_string->chars[i] == USED_QUOTATION)
        {
            json_string->chars = setchar_at(json_string->chars, DOUBLE_QUOTE, i, strlen(json_string->chars));
        }

        if (json_string->chars[i] == DOUBLE_QUOTE)
        {

            if (USED_QUOTATION == DOUBLE_QUOTE)
                json_string->chars = setchar_at(json_string->chars, SINGLE_QUOTE, i, strlen(json_string->chars));
            if (USED_QUOTATION == SINGLE_QUOTE)
                json_string->chars = setchar_at(json_string->chars, DOUBLE_QUOTE, i, strlen(json_string->chars));
        }
    }

    // puts(json_string->chars);

    int inquotes = 0;
    string_t *j_2 = string_create();

    for (size_t i = 0; i < strlen(json_string->chars); i++)
    {
        if (json_string->chars[i] == DOUBLE_QUOTE && inquotes == 0)
        {
            inquotes = 1;
            string_append(j_2, json_string->chars[i]);
            continue;
        }

        if (inquotes == 0 && (json_string->chars[i] == SPACE || json_string->chars[i] == TAB || json_string->chars[i] == LINE_BREAK))
        {
            continue;
        }

        string_append(j_2, json_string->chars[i]);

        if (json_string->chars[i] == DOUBLE_QUOTE && inquotes == 1)
        {
            inquotes = 0;
        }
    }

    string_destroy(json_string);

    return j_2->chars;
}

void *json_parse(char *json_string)
{
    json_string = json_check_syntax(json_string);

    if (is_array)
        return json_array(json_string);

    return json_object(json_string);
}

json_array_t *json_array(char *json_string)
{

    json_array_t *j_array = malloc(sizeof(json_array_t));

    char *jtmp = json_string;
    json_string = string_removechar_at(0, json_string, strlen(json_string));
    free(jtmp);
    jtmp = json_string;
    json_string = string_removechar_at(strlen(json_string) - 1, json_string, strlen(json_string));
    free(jtmp);

    replace_comma(&json_string);

    list_t *objects = split(COMMA, json_string, strlen(json_string));
    j_array->j_objects = objects;
    free(json_string);
    // puts(json_string);

    return j_array;
}

json_object_t *json_object(char *json_string)
{
    map_t *json_pairs = map_create();
    json_object_t *j_object = malloc(sizeof(json_object_t));

    char *jtmp = json_string;
    json_string = string_removechar_at(0, json_string, strlen(json_string));
    free(jtmp);
    jtmp = json_string;
    json_string = string_removechar_at(strlen(json_string) - 1, json_string, strlen(json_string));
    free(jtmp);

    replace_comma(&json_string);

    list_t *list_of_pairs = split(COMMA, json_string, strlen(json_string));
    list_t *tmp = list_of_pairs;

    while (tmp != NULL)
    {
        list_t *this_pair = split_lim(COLON, tmp->value, strlen(tmp->value), 2);
        if (list_len(this_pair) != 2)
            continue;

        char *key = string_removechar(DOUBLE_QUOTE,
                                      list_get(this_pair, 0),
                                      strlen(list_get(this_pair, 0)));
        char *ktmp = key;

        key = string_removechar(SINGLE_QUOTE,
                                      list_get(this_pair, 0),
                                      strlen(list_get(this_pair, 0)));

        free(ktmp);

        /*char *value = string_removechar(DOUBLE_QUOTE,
                                        list_get(this_pair, 1),
                                        strlen(list_get(this_pair, 1)));
        char *vtmp = value;*/
        char *value = string_replacechar(SPECIAL, COMMA, list_get(this_pair, 1),
                                        strlen(list_get(this_pair, 1)));
        //free(vtmp);

        map_add(json_pairs, key, value);
        free(key);
        free(value);
        tmp = tmp->next;
    }

    j_object->j_pairs = json_pairs;

    list_destroy(list_of_pairs);

    return j_object;
}

void replace_comma(char **arg)
{
    bool isJsonArray = false;
    int brackets = 0;

    for (size_t i = 0; i < strlen(*arg); i++)
    {

        if ((*arg)[i] == SQUARE_CLOSE_BRACKETS)
        {
            brackets--;
            if (brackets == 0)
            {
                isJsonArray = false;
            }
        }

        if (isJsonArray && COMMA == (*arg)[i])
        {
            *arg = setchar_at(*arg, SPECIAL, i, strlen(*arg));
            // cout << (*arg)[i]<<endl;
        }

        if ((*arg)[i] == SQUARE_OPEN_BRACKETS)
        {
            isJsonArray = true;
            brackets++;
        }
    }
}
