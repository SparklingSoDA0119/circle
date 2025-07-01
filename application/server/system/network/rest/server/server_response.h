#ifndef _CIRCLE_SERVER_SYSTEM_NETWORK_REST_SERVER_SERVER_RESPONSE_H_
#define _CIRCLE_SERVER_SYSTEM_NETWORK_REST_SERVER_SERVER_RESPONSE_H_

#include "define.h"

_namespace_server_begin

class Response
{
public :
	Response();
	Response(const int32_t code, const String& msg = L"");
	Response(const Response& resp);

	virtual ~Response();

public :
	int32_t code() const;
	const String& msg() const;

	void setCode(int32_t code);
	void setMsg(const String& msg);

public :
	bool operator==(const Response& resp) const;
	bool operator!=(const Response& resp) const;

	Response& operator=(const Response& resp);

private :
	int32_t _code;
	String _msg;
};	// class Response

inline int32_t Response::code() const { return _code; }

inline const String& Response::msg() const { return _msg; }


inline void Response::setCode(int32_t code) { _code = code; }

inline void Response::setMsg(const String& msg) { _msg = msg; }

_namespace_server_end

#endif