#pragma once

#include "http.h"

#define GUID "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"

bool validate_WS_connection(map_t *request);

char *createAcceptString(char *ws_key);

int createIntFromByte(int *bytes, size_t len);

