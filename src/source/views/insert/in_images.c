#include <in_images.h>

void insert_user_image(int sock,char *url,char *filename)
{
    char * user_id = get_param_from_url(url,"id");

    if(user_id == NULL)
    {
        write_BAD(sock);
        return;
    }

    json_object * jobj =  insert_user_image_filename(user_id,filename);

    jobj == NULL ? write_404(sock) :  write_json(jobj,sock) ;
    json_object_put(jobj);
    free(user_id);
}


void insert_pub_image(int sock,char *url,char *filename)
{
    char * user_id = get_param_from_url(url,"id");

    if(user_id == NULL)
    {
        write_BAD(sock);
        return;
    }

    json_object * jobj =  insert_pub_image_filename(user_id,filename);

    jobj == NULL ? write_404(sock) :  write_json(jobj,sock) ;
    json_object_put(jobj);
    free(user_id);
}