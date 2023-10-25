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

#include "in_images.h"

void method_get(SSL *sock, char *url);

void method_put(SSL *sock, char *url, char *json);

void method_post(SSL *sock, char *url, char *json);

void method_delete(SSL *sock, char *url);

void method_post_file(SSL *sock,char *url,char *filename);