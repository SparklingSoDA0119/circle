#ifndef _CIRCLE_SERVER_SYSTEM_NETWORK_REST_REST_DEFINE_H_
#define _CIRCLE_SERVER_SYSTEM_NETWORK_REST_REST_DEFINE_H_

#include "define.h"

_namespace_server_begin

enum class Method
{
	Undefined = 0,
	Get,
	Post,
	Put,
	Delete,
};

#if defined(_WIN64)
	#define REST_STRING(x) x.to_wstring()
#else
	#define REST_STRING(x) x.to_string()
#endif

_namespace_server_end

#endif	// _CIRCLE_SERVER_SYSTEM_NETWORK_REST_REST_DEFINE_H_