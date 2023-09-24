#include "../include/index.h"

void login(char *url, int sock)
{
    map_t *url_m = parse_url(url);
    if (url_m == NULL)
    {
        write_BAD(sock);
        return;
    }

    if (map_len(url_m) != 2)
    {
        write_BAD(sock);
        map_destroy(url_m);
        return;
    }
    puts(map_get(url_m, "query"));

    map_t *params = parse_url_query(map_get(url_m, "query"));

    puts("parsed");

    if (params == NULL)
    {

        write_BAD(sock);
        return;
    }

    if (map_len(params) != 2)
    {
        write_BAD(sock);
        map_destroy(url_m);
        return;
    }

    bool log_wit_email = true;
    char *colname = map_get(params, "email");

    if (colname == NULL)
    {
        puts("is nill");
        log_wit_email = false;
        colname = map_get(params, "username");
    }

    if (colname == NULL || !map_get(params, "password"))
    {
        write_BAD(sock);
        return;
    }

    map_t *res = map_create();
    json_object *jb = NULL;

    if (!check_if_user_exists(
            log_wit_email ? map_get(params, "email") : map_get(params, "username"),
            map_get(params, "password"),
            log_wit_email ? true : false))
    {
        map_add(res, "found", "false");
        jb = create_json_object_from_map(res);
        write_json(jb, sock);
        goto clean_up;
    }

    map_add(res, "found", "true");
    jb = create_json_object_from_map(res);
    write_json(jb, sock);

clean_up:
    json_object_put(jb);
    map_destroy(params);
    map_destroy(url_m);
    map_destroy(res);
}

void sign_up(int sock, char *json_load)
{
    // write_404(sock);
    json_object *jobj = json_tokener_parse(json_load);
    json_object *name, *username, *email, *password;

    if (!json_object_object_get_ex(jobj, "name", &name))
    {
        write_BAD(sock);
        // todo
        return;
    }

    if (!json_object_object_get_ex(jobj, "username", &username))
    {
        write_BAD(sock);
        // todo
        return;
    }

    if (!json_object_object_get_ex(jobj, "email", &email))
    {
        write_BAD(sock);
        // todo
        return;
    }

    if (!json_object_object_get_ex(jobj, "password", &password))
    {
        write_BAD(sock);
        // todo
        return;
    }

    // write_404(sock);

    map_t *res = map_create();
    json_object *j_res = NULL;

    if (check_if_user_data_exists(json_object_get_string(username), json_object_get_string(email)))
    {
        map_add(res, "success", "exists");
        j_res = create_json_object_from_map(res);
        write_json(j_res, sock);
        goto clean_up;
    }

    if (!inser_into_users(json_object_get_string(name),
                          json_object_get_string(username),
                          json_object_get_string(email),
                          json_object_get_string(password)))
    {
        map_add(res, "success", "false");
        j_res = create_json_object_from_map(res);
        write_json(j_res, sock);
        goto clean_up;
    }

    map_add(res, "success", "true");
    j_res = create_json_object_from_map(res);
    write_json(j_res, sock);

clean_up:
    json_object_put(j_res);
}

void add_event(int sock, char *json_load)
{

    // write_404(sock);
    json_object *jobj = json_tokener_parse(json_load);
    json_object *title, *description, *venue, *event_date, *deadline_date, *publisher_id;

    if (!json_object_object_get_ex(jobj, "title", &title))
    {
        write_BAD(sock);
        // todo
        return;
    }

    if (!json_object_object_get_ex(jobj, "description", &description))
    {
        write_BAD(sock);
        // todo
        return;
    }

    if (!json_object_object_get_ex(jobj, "venue", &venue))
    {
        write_BAD(sock);
        // todo
        return;
    }

    if (!json_object_object_get_ex(jobj, "event_date", &event_date))
    {
        write_BAD(sock);
        // todo
        return;
    }

    if (!json_object_object_get_ex(jobj, "deadline_date", &deadline_date))
    {
        write_BAD(sock);
        // todo
        return;
    }

    if (!json_object_object_get_ex(jobj, "publisher_id", &publisher_id))
    {
        write_BAD(sock);
        // todo
        return;
    }

    // write_404(sock);

    map_t *res = map_create();
    json_object *j_res = NULL;

    if (!insert_into_pubished(json_object_get_string(title),
                              json_object_get_string(description),
                              json_object_get_string(venue),
                              json_object_get_string(event_date),
                              json_object_get_string(deadline_date),
                              json_object_get_string(publisher_id)))
    {
        map_add(res, "success", "false");
        j_res = create_json_object_from_map(res);
        write_json(j_res, sock);
        goto clean_up;
    }

    map_add(res, "success", "true");
    j_res = create_json_object_from_map(res);
    write_json(j_res, sock);

clean_up:
    json_object_put(j_res);
}

void add_star(int sock, char *json_load)
{
    // write_404(sock);
    json_object *jobj = json_tokener_parse(json_load);
    json_object *user_id, *publish_id;

    if (!json_object_object_get_ex(jobj, "user_id", &user_id))
    {
        write_BAD(sock);
        // todo
        return;
    }

    if (!json_object_object_get_ex(jobj, "publish_id", &publish_id))
    {
        write_BAD(sock);
        // todo
        return;
    }
    // write_404(sock);

    map_t *res = map_create();
    json_object *j_res = NULL;

    if (!insert_into_stars(json_object_get_string(user_id),
                           json_object_get_string(publish_id)))
    {
        map_add(res, "success", "false");
        j_res = create_json_object_from_map(res);
        write_json(j_res, sock);
        goto clean_up;
    }

    map_add(res, "success", "true");
    j_res = create_json_object_from_map(res);
    write_json(j_res, sock);

clean_up:
    json_object_put(j_res);
}

void add_subscriber(int sock, char *json_load)
{
    // write_404(sock);
    json_object *jobj = json_tokener_parse(json_load);
    json_object *user_id, *publish_id;

    if (!json_object_object_get_ex(jobj, "user_id", &user_id))
    {
        write_BAD(sock);
        // todo
        return;
    }

    if (!json_object_object_get_ex(jobj, "publish_id", &publish_id))
    {
        write_BAD(sock);
        // todo
        return;
    }
    // write_404(sock);

    map_t *res = map_create();
    json_object *j_res = NULL;

    if (!insert_into_subscribers(json_object_get_string(user_id),
                                 json_object_get_string(publish_id)))
    {
        map_add(res, "success", "false");
        j_res = create_json_object_from_map(res);
        write_json(j_res, sock);
        goto clean_up;
    }

    map_add(res, "success", "true");
    j_res = create_json_object_from_map(res);
    write_json(j_res, sock);

clean_up:
    json_object_put(j_res);
}

void add_follower(int sock, char *json_load)
{
    // write_404(sock);
    json_object *jobj = json_tokener_parse(json_load);
    json_object *user_id, *follower_id;

    if (!json_object_object_get_ex(jobj, "user_id", &user_id))
    {
        write_BAD(sock);
        // todo
        return;
    }

    if (!json_object_object_get_ex(jobj, "follower_id", &follower_id))
    {
        write_BAD(sock);
        // todo
        return;
    }
    // write_404(sock);

    map_t *res = map_create();
    json_object *j_res = NULL;

    if (!insert_into_followers(json_object_get_string(user_id),
                               json_object_get_string(follower_id)))
    {
        map_add(res, "success", "false");
        j_res = create_json_object_from_map(res);
        write_json(j_res, sock);
        goto clean_up;
    }

    map_add(res, "success", "true");
    j_res = create_json_object_from_map(res);
    write_json(j_res, sock);

clean_up:
    json_object_put(j_res);
}

void get_events(int sock, char *json_load)
{
    // write_404(sock);
    json_object *jobj = json_tokener_parse(json_load);
    json_object *res = NULL;
    json_object *user_id, *last_time, *last_query_time;

    if (!json_object_object_get_ex(jobj, "user_id", &user_id))
    {
        write_BAD(sock);
        // todo
        return;
    }

    if (!json_object_object_get_ex(jobj, "last_time", &last_time))
    {
        write_BAD(sock);
        // todo
        return;
    }

    if (!json_object_object_get_ex(jobj, "last_query_time", &last_query_time))
    {
        write_BAD(sock);
        // todo
        return;
    }

    res = select_from_published(json_object_get_string(user_id),
                                json_object_get_string(last_time),
                                json_object_get_string(last_query_time));

    if (res)
    {
        write_json(res, sock);
        json_object_put(res);
    }
    else
    {
        write_404(sock);
    };

    json_object_put(jobj);
}

void get_one_event(int sock, char *json_load)
{
    json_object *jobj = json_tokener_parse(json_load);
    json_object *res = NULL;
    json_object *id;

    if (!json_object_object_get_ex(jobj, "id", &id))
    {
        write_BAD(sock);
        // todo
        return;
    }

    res = select_one_from_published(json_object_get_string(id));

    if (res)
    {
        write_json(res, sock);
        json_object_put(res);
    }
    else
    {
        write_404(sock);
    };

    json_object_put(jobj);
}


void update_event(int sock,char *json_load)
{

    // write_404(sock);
    json_object *jobj = json_tokener_parse(json_load);
    json_object *title, *description, *venue, *event_date, *deadline_date, *publisher_id,*id;

    if (!json_object_object_get_ex(jobj, "title", &title))
    {
        write_BAD(sock);
        // todo
        puts("mising title");
        return;
    }

    if (!json_object_object_get_ex(jobj, "description", &description))
    {
        write_BAD(sock);
        // todo
        puts("mising desc");
        return;
    }

    if (!json_object_object_get_ex(jobj, "venue", &venue))
    {
        write_BAD(sock);
        // todo
        puts("mising venue");
        return;
    }

    if (!json_object_object_get_ex(jobj, "event_date", &event_date))
    {
        write_BAD(sock);
        // todo
        puts("mising event date");
        return;
    }

    if (!json_object_object_get_ex(jobj, "deadline_date", &deadline_date))
    {
        write_BAD(sock);
        // todo
        puts("mising deadline");
        return;
    }

    if (!json_object_object_get_ex(jobj, "publisher_id", &publisher_id))
    {
        write_BAD(sock);
        // todo
        puts("mising pulisher");
        return;
    }

    if (!json_object_object_get_ex(jobj, "id", &id))
    {
        write_BAD(sock);
        // todo
        puts("mising id");
        return;
    }

    // write_404(sock);

    map_t *res = map_create();
    json_object *j_res = NULL;

    if (!update_pubished(json_object_get_string(title),
                              json_object_get_string(description),
                              json_object_get_string(venue),
                              json_object_get_string(event_date),
                              json_object_get_string(deadline_date),
                              json_object_get_string(publisher_id), json_object_get_string(id)))
    {
        map_add(res, "success", "false");
        j_res = create_json_object_from_map(res);
        write_json(j_res, sock);
        goto clean_up;
    }

    map_add(res, "success", "true");
    j_res = create_json_object_from_map(res);
    write_json(j_res, sock);

clean_up:
    json_object_put(j_res);
}


void update_user(int sock,char *json_load)
{
        // write_404(sock);
    json_object *jobj = json_tokener_parse(json_load);
    json_object *name, *username, *avater, *bio, *about, *email,*id;

    if (!json_object_object_get_ex(jobj, "name", &name))
    {
        write_BAD(sock);
        // todo
        puts("mising title");
        return;
    }

    if (!json_object_object_get_ex(jobj, "username", &username))
    {
        write_BAD(sock);
        // todo
        puts("mising desc");
        return;
    }

    if (!json_object_object_get_ex(jobj, "avater", &avater))
    {
        write_BAD(sock);
        // todo
        puts("mising venue");
        return;
    }

    if (!json_object_object_get_ex(jobj, "bio", &bio))
    {
        write_BAD(sock);
        // todo
        puts("mising event date");
        return;
    }

    if (!json_object_object_get_ex(jobj, "about", &about))
    {
        write_BAD(sock);
        // todo
        puts("mising deadline");
        return;
    }

    if (!json_object_object_get_ex(jobj, "email", &email))
    {
        write_BAD(sock);
        // todo
        puts("mising pulisher");
        return;
    }

    if (!json_object_object_get_ex(jobj, "id", &id))
    {
        write_BAD(sock);
        // todo
        puts("mising id");
        return;
    }

    // write_404(sock);

    map_t *res = map_create();
    json_object *j_res = NULL;

    if (!update_one_user(json_object_get_string(name),
                              json_object_get_string(username),
                              json_object_get_string(email),
                              json_object_get_string(avater),
                              json_object_get_string(bio),
                              json_object_get_string(about), json_object_get_string(id)))
    {
        map_add(res, "success", "false");
        j_res = create_json_object_from_map(res);
        write_json(j_res, sock);
        goto clean_up;
    }

    map_add(res, "success", "true");
    j_res = create_json_object_from_map(res);
    write_json(j_res, sock);

clean_up:
    json_object_put(j_res);
}