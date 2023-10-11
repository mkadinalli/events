#pragma once

#include <http_client.h>
#include <string.h>
#include <stdbool.h>

#define BUFFLEN 4096

bool gmail_send_message(char *receiver_email,char *message);