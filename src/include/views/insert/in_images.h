#pragma once

#include "../../../include/views/select/get-events.h"
#include "../../../include/lib/urls.h"
#include "ev_media.h"

#include <stdio.h>
#include <string.h>

void insert_user_image(SSL *sock,char *url,char *filename);
void insert_pub_image(SSL *sock,char *url,char *filename);