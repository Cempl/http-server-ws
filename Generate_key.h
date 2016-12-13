#pragma once


/*******************************************************************************/
#include "Server.h"


/*******************************************************************************/
string base64_encode(unsigned char const*, unsigned int len);


/*******************************************************************************/
const string guid = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";

static const string base64_chars =
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz"
"0123456789+/";


/*******************************************************************************/
static inline bool is_base64(unsigned char c)
{
	return (isalnum(c) || (c == '+') || (c == '/'));
}
