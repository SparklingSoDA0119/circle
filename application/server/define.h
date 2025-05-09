#ifndef _CIRCLE_SERVER_DEFINE_H_
#define _CIRCLE_SERVER_DEFINE_H_

#include <iostream>
#include <string>
#include <memory>
#include <vector>

#define _namespace_circle_begin	namespace cc {
#define _namespace_circle_end	}

#define _namespace_server_begin _namespace_circle_begin namespace srv {
#define _namespace_server_end   }	_namespace_circle_end

_namespace_server_begin

#define _M_server_version		"0.0.1"
#define _M_server_build_number	"0001"

_namespace_server_end

#endif	// _CIRCLE_SERVER_DEFINE_H_