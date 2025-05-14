#ifndef _CIRCLE_SERVER_DEFINE_H_
#define _CIRCLE_SERVER_DEFINE_H_

#include <libsoda/foundation/string.h>

#include <iostream>
#include <string>
#include <memory>
#include <vector>

#if defined(_WIN64)
	#pragma warning(disable: 4996)
#endif

#define _namespace_circle_begin	namespace cc {
#define _namespace_circle_end	}

#define _namespace_server_begin _namespace_circle_begin namespace srv {
#define _namespace_server_end   }	_namespace_circle_end

_namespace_server_begin

using soda::String;

#define _M_server_version		L"0.0.1"
#define _M_server_build_number	L"0001"

_namespace_server_end

#endif	// _CIRCLE_SERVER_DEFINE_H_