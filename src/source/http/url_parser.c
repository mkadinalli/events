#include "url_parser.h"

url_t *url_parser_parse(char *url)
{
    if(!verify_url(url)) {
        puts("invalid url");
        return NULL;
        }

    url_t * url_obj = malloc(sizeof(url_t));
    
    /*url_obj->domain = NULL;
    url_obj->params = NULL;
    url_obj->scheme = NULL;
    url_obj->port = NULL;
    url_obj->path = NULL;*/

    memset(url_obj,0,sizeof(url_t));

    url_obj->domain = get_domain_name_from_url(url);
    url_obj->path = get_path_from_url(url);

    char *p = get_param_part_from_url(url);

    url_obj->params = parse_url_query(p);

    url_obj->port = get_port_from_url(url);

    string_t * ww = string_create_from_string("www.");
    string_concat(ww,url_obj->domain,strlen(url_obj->domain));

    url_obj->www_domain = ww->chars;

    free(ww);

    free(p);

    return url_obj;
}

void url_free(url_t *url)
{
    if(url == NULL) return;

    if(url->domain) free(url->domain);
    if(url->params) map_destroy(url->params);
    if(url->path) free(url->path);
    if(url->port) free(url->port);
    if(url->scheme) free(url->scheme);
}