#pragma once

#include "in-events.h"
#include "in-followers.h"
#include "in-stars.h"
#include "in-subscriptions.h"
#include "in-users.h"
#include "in-payment.h"

#include "get-events.h"
#include "get-followers.h"
#include "get-stars.h"
#include "get-subscriptions.h"
#include "get-users.h"

#include "update-event.h"
#include "update-user.h"

#include <openssl/ssl.h>
#include <openssl/err.h>

#include "map.h"
#include "ev_strings.h"
#include "delete_.h"

#include "in_images.h"

void method_get(int sock, char *url);

void method_put(int sock, char *url, char *json);

void method_post(int sock, char *url, char *json);

void method_delete(int sock, char *url);

void method_post_file(int sock,char *url,char *content_type,int content_length);

void method_get_ws(int sock,map_t *req);