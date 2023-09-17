#include "../include/index.h"

void login(char *url,int sock){
    

    puts("here");

    map_t * url_m = parse_url(url);
    if(url_m == NULL){
        write_BAD(sock);
        return;
    }


    if(map_len(url_m) != 2)
    {
        write_BAD(sock);
        puts("destroy");
        map_destroy(url_m);
        return;
    }

    puts("one step");

    map_t *params = parse_url_query(map_get(url_m,"query"));


    if(params == NULL){
        write_BAD(sock);
        return;
    }

    map_print(params);
    puts("-----");

    if(map_len(params) != 2)
    {
        write_BAD(sock);
        map_destroy(url_m);
        return;
    }

    bool log_wit_email = true;
    char *colname = map_get(params,"email");

    if(colname == NULL)
    {
        puts("is nill");
        log_wit_email = false;
        colname = map_get(params,"username");
    }

    puts(colname);

    if(colname == NULL || !map_get(params,"value")){
         write_BAD(sock);
         return;
    }

    puts("exit");

    /*
    //===========================================
    MYSQL * db_conn = NULL;
    db_conn = mysql_init(db_conn);
    db_conn = create_connection_from_a_file(db_conn,
                                  "/home/vic/Desktop/ev2/events/config/config.json");

    if(db_conn == NULL)
    {
        puts("failed to connect to db");
        return;
    }
    //===========================================

    map_t * res = map_create();
    json_object * jb = NULL;

    if(!check_if_data_exists(db_conn,"users",
    map_get(params,log_wit_email ? "email":"username"),map_get(params,"value")))
    {
        map_add(res,"found","true");
        jb = create_json_object_from_map(res);
        write_json(jb,sock);
        goto clean_up;
    }

    map_add(res,"found","false");
    jb = create_json_object_from_map(res);
    write_json(jb,sock);

    clean_up:
    mysql_close(db_conn);
    json_object_put(jb);
    map_destroy(params);
    map_destroy(url_m);
    map_destroy(res);*/
}