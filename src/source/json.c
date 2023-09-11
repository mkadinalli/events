#include "../include/json.h"

char USED_QUOTATION = ' ';

void
json_check_syntax(char *j_string)
{
    string_t * json_string = string_create_from_string(j_string);

    bool is_array = false;

    if(endswith(SQUARE_CLOSE_BRACKETS,json_string->chars) &&
     startswith(SQUARE_OPEN_BRACKETS,json_string->chars)){
        is_array = true;
	}

    if(is_array){ json_check_arr_syntax(json_string->chars); }

    else

    { json_check_obj_syntax(json_string->chars); }


}

void json_check_obj_syntax(char * json_string){
    for(size_t i = 0;i<strlen(json_string);i++){
		if(json_string[i] == SINGLE_QUOTE || json_string[i] == DOUBLE_QUOTE){
			USED_QUOTATION = json_string[i];
			break;
		}

	}

    if(!(endswith(CURLY_CLOSE_BRACKETS,json_string)) ||
     !(startswith(CURLY_OPEN_BRACKETS,json_string))){
		puts("Unsupported File Format");
		return;
	}
	
	if(USED_QUOTATION == ' '){
		puts("Unsupported File Format");
		return;
	}
}

void json_check_arr_syntax(char * json_string){
    for(size_t i = 0;i<strlen(json_string);i++){
		if(json_string[i] == SINGLE_QUOTE || json_string[i] == DOUBLE_QUOTE){
			USED_QUOTATION = json_string[i];
			break;
		}

	}

    if(!(endswith(SQUARE_CLOSE_BRACKETS,json_string)) ||
     !(startswith(SQUARE_OPEN_BRACKETS,json_string))){
		puts("Unsupported File Format");
		return;
	}
	
	if(USED_QUOTATION == ' '){
		puts("Unsupported File Format");
		return;
	}
}