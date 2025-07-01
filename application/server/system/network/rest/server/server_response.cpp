#include "system/network/rest/server/server_response.h"

_namespace_server_begin

Response::Response()
	: _code(200)
	, _msg(L"success")
{

}


Response::Response(const int32_t code, const String& msg)
	: _code(code)
	, _msg(msg)
{

}


Response::Response(const Response& resp)
	: Response()
{
	*this = resp;
}


Response::~Response()
{

}


bool Response::operator==(const Response& resp) const
{
	if (_code == resp._code &&
		_msg == resp._msg)
	{
		return true;
	}
	else {
		return false;
	}
}


bool Response::operator!=(const Response& resp) const
{
	return !(*this == resp);
}


Response& Response::operator=(const Response& resp)
{
	_code = resp._code;
	_msg = resp._msg;

	return *this;
}

_namespace_server_end