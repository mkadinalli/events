#include "../include/json.h"

bool is_array = false;
char *json_check_syntax(char *j_string)
{
    string_t *json_string = string_create_from_string(j_string);
    string_t *curls = string_create();
    string_t *squares = string_create();
    string_t *symbols = string_create();

    int line = 1, col = 1;

    // string_append(symbols,' ');

    bool quotes = false;

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

    for (size_t i = 0; i < strlen(json_string->chars); i++)
    {
        // string_append(symbols, json_string->chars[i]);

        if (json_string->chars[i] == LINE_BREAK)
        {
            line++;
            col = 1;
        }

        if (json_string->chars[i] == USED_QUOTATION && !quotes)
        {
            quotes = true;
            string_append(symbols, json_string->chars[i]);
        }

        if (!quotes)
        {

            if (json_string->chars[i] == SQUARE_OPEN_BRACKETS)
            {
                if (string_back(symbols) == CURLY_OPEN_BRACKETS ||
                    string_back(symbols) == CURLY_CLOSE_BRACKETS ||
                    string_back(symbols) == SQUARE_OPEN_BRACKETS ||
                    string_back(symbols) == USED_QUOTATION ||
                    (string_back(symbols) >= '0' && string_back(symbols) <= '9'))
                {
                    printf("1 syntax err [%d , %d]\n", line, col);
                    return NULL;
                }
                string_append(squares, json_string->chars[i]);
            }

            if (json_string->chars[i] == SQUARE_CLOSE_BRACKETS)
            {

                if (string_back(squares) == SQUARE_OPEN_BRACKETS)
                {
                    string_pop(squares);
                }
                else
                {
                    string_append(squares, json_string->chars[i]);
                }
            }

            if (json_string->chars[i] == CURLY_OPEN_BRACKETS)
            {
                if (string_back(symbols) == CURLY_OPEN_BRACKETS ||
                    string_back(symbols) == CURLY_CLOSE_BRACKETS ||
                    (string_back(symbols) == SQUARE_OPEN_BRACKETS && !is_array) ||
                    string_back(symbols) == USED_QUOTATION ||
                    (string_back(symbols) >= '0' && string_back(symbols) <= '9'))
                {
                    printf("2 syntax err %s [%d , %d]\n", symbols->chars, line, col);
                    return NULL;
                }
                string_append(curls, json_string->chars[i]);
            }
            if (json_string->chars[i] == COMMA && string_back(symbols) == CURLY_OPEN_BRACKETS)
            {
                printf("3 syntax err [%d , %d]\n", line, col);
                return NULL;
            }

            if (json_string->chars[i] == COLON && (string_back(symbols) == CURLY_CLOSE_BRACKETS ||
                                                   string_back(symbols) == SQUARE_CLOSE_BRACKETS ||
                                                   string_back(symbols) == COLON ||
                                                   string_back(symbols) != USED_QUOTATION))
            {
                printf("4 syntax err %c [%d , %d]\n", string_back(symbols), line, col);
                exit(1);
            }

            if (json_string->chars[i] == CURLY_CLOSE_BRACKETS)
            {
                if (string_back(symbols) == COMMA)
                {
                    printf("5 syntax err [%d , %d]\n", line, col);
                    exit(1);
                }
                if (string_back(curls) == CURLY_OPEN_BRACKETS)
                {
                    string_pop(curls);
                }
                else
                {
                    string_append(curls, json_string->chars[i]);
                }
            }

            if (json_string->chars[i] == COMMA ||
                json_string->chars[i] == COLON ||
                json_string->chars[i] == SQUARE_CLOSE_BRACKETS ||
                json_string->chars[i] == SQUARE_OPEN_BRACKETS ||
                json_string->chars[i] == CURLY_CLOSE_BRACKETS ||
                json_string->chars[i] == CURLY_OPEN_BRACKETS ||
                (json_string->chars[i] >= '0' && json_string->chars[i] <= '9'))
            {
                string_append(symbols, json_string->chars[i]);
            }
        }

        if (json_string->chars[i] == USED_QUOTATION && quotes)
        {
            quotes = false;
            string_append(symbols, json_string->chars[i]);
        }

        col++;
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

    list_print(list_of_pairs);
    list_t *tmp = list_of_pairs;

    while (tmp != NULL)
    {
        list_t *this_pair = split_lim(COLON, tmp->value, strlen(tmp->value), 2);
        if (list_len(this_pair) != 2)
            continue;

        char *key = string_removechar(DOUBLE_QUOTE,
                                      list_get(this_pair, 0),
                                      strlen(list_get(this_pair, 0)));

        char *value = string_removechar(DOUBLE_QUOTE,
                                      list_get(this_pair, 1),
                                      strlen(list_get(this_pair, 1)));
        map_add(json_pairs,key,value);
        tmp = tmp->next;
    }

    j_object->j_pairs = json_pairs;

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
