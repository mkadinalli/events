#pragma once

#include "../../http/requests.h"
#include "../../lib/ev_strings.h"
#include "../../data-structures/map.h"
#include "../../da/payment-da.h"

#include "mpesa.h"

void add_payment(SSL *sock, char *json_load);