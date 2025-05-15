#include "system/db/db_session_param.h"

_namespace_server_begin

DbSessionParam::DbSessionParam()
	: _host()
	, _port(0)
	, _dbName()
	, _id()
	, _pw()
	, _useAutoReConnect(true)
{

}


DbSessionParam::DbSessionParam(const DbSessionParam& param)
	: DbSessionParam()
{
	*this = param;
}


DbSessionParam::~DbSessionParam()
{

}


const String DbSessionParam::getConnectString() const
{
	String s;

	s.appendf(L"host=%s;", _host.to_string().c_str());
	s.appendf(L"port=%d;", _port);
	s.appendf(L"db=%s;", _dbName.to_string().c_str());
	s.appendf(L"user=%s;password=%s;", _id.to_string().c_str(), _pw.to_string().c_str());
	s.appendf(L"auto-reconnect=%s", _useAutoReConnect ? "true" : "false");

	return s;
}


bool DbSessionParam::operator==(const DbSessionParam& param) const
{
	if (_host == param._host &&
		_port == param._port &&
		_dbName == param._dbName &&
		_id == param._dbName &&
		_pw == param._pw &&
		_useAutoReConnect == param._useAutoReConnect)
	{
		return true;
	}
	else {
		return false;
	}
}


bool DbSessionParam::operator!=(const DbSessionParam& param) const
{
	return !operator==(param);
}


DbSessionParam& DbSessionParam::operator=(const DbSessionParam& param)
{
	_host = param._host;
	_port = param._port;
	_dbName = param._dbName;
	_id = param._id;
	_pw = param._pw;
	_useAutoReConnect = param._useAutoReConnect;

	return *this;
}

_namespace_server_end