#include "system/network/rest/rest_server_param.h"

_namespace_server_begin

const String toString(const NetScheme eScheme)
{
	switch (eScheme)
	{
	case NetScheme::HTTP:	return L"HTTP";
	case NetScheme::HTTPS:	return L"HTTPS";
	default:				return L"Undefined";
	}
}


RestServerParam::RestServerParam()
	: _addr(L"127.0.0.1")
	, _port(4817)
	, _baseUrl(L"unknown")
	, _timeoutSec(15)
	, _eScheme(NetScheme::HTTP)
{

}


RestServerParam::RestServerParam(const RestServerParam& param)
	: RestServerParam()
{
	*this = param;
}


RestServerParam::~RestServerParam()
{

}


const web::uri_builder RestServerParam::getUrlBuilder() const
{
	web::uri_builder builder;
	
	builder.set_scheme(srv::toString(_eScheme).to_wstring());
	builder.set_host(_addr.to_wstring());
	builder.set_port(std::to_wstring(_port));
	builder.set_path(_baseUrl.to_wstring());

	return builder;
}


bool RestServerParam::operator==(const RestServerParam& param) const
{
	if (_addr == param._addr &&
		_port == param._port &&
		_baseUrl == param._baseUrl &&
		_timeoutSec == param._timeoutSec &&
		_eScheme == param._eScheme)
	{
		return true;
	}
	else {
		return false;
	}
}


bool RestServerParam::operator!=(const RestServerParam& param) const
{
	return !operator==(param);
}


RestServerParam& RestServerParam::operator=(const RestServerParam& param)
{
	_addr = param._addr;
	_port = param._port;
	_baseUrl = param._baseUrl;
	_timeoutSec = param._timeoutSec;
	_eScheme = param._eScheme;
	
	return *this;
}


const String RestServerParam::toString() const
{
	String s;
	s.appendf(L"%s://%s:%u/%s (timeout: %u)", srv::toString(_eScheme).cString(),
		_addr.cString(), _port, _baseUrl.cString(), _timeoutSec);

	return s;
}

_namespace_server_end