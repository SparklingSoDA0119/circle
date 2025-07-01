#include "system/network/rest/rest_request.h"

_namespace_server_begin

RestRequest::RestRequest()
	: _uri()
	, _relativeUri()
	, _path()
	, _ip()
	, _contentType()
	, _body()
	, _queryStr()
	, _eMethod(Method::Get)
	, _headers{}
	, _querys{}
	, _request()
{

}


RestRequest::RestRequest(const RestRequest& req)
	: RestRequest()
{
	*this = req;
}


RestRequest::RestRequest(const web::http::http_request& req)
	: RestRequest()
{
	_uri = web::uri::decode(req.request_uri().to_string());
	_relativeUri = web::uri::decode(req.relative_uri().to_string());
	_ip = req.remote_address();
	_contentType = req.headers().content_type();
	_queryStr = web::uri::decode(req.request_uri().query());
	_path = web::uri::decode(req.request_uri().path());

	if (req.method() == web::http::methods::GET) { _eMethod = Method::Get; }
	else if (req.method() == web::http::methods::POST) { _eMethod = Method::Post; }
	else if (req.method() == web::http::methods::PUT) { _eMethod = Method::Put; }
	else if (req.method() == web::http::methods::DEL) { _eMethod = Method::Delete; }
	else {
		_eMethod = Method::Undefined;
	}

	for (auto itr = req.headers().begin(); itr != req.headers().end(); ++itr) {
		_headers.insert(std::pair<String, String>(itr->first.c_str(), itr->second.c_str()));
	}

	parseQuery();

	_request = req;
}


RestRequest::~RestRequest()
{

}


void RestRequest::parseQuery()
{
	if (_queryStr.isEmpty()) {
		return;
	}

	size_t sPos = 0;
	while (true) {
		try {
			size_t ePos = _queryStr.find(L"&", sPos);
			if (ePos == String::npos) {
				ePos = _queryStr.length();
			}

			size_t equalPos = _queryStr.find(L"=", sPos);
			try {
				String key = _queryStr.substring(sPos, equalPos - sPos);
				String val = _queryStr.substring(equalPos + 1, ePos - equalPos - 1);
				sPos = ePos + 1;
				_querys.insert(std::make_pair(key, val));
				if (sPos > _queryStr.length()) {
					break;
				}
			}
			catch (...) {
				return;
			}
		}
		catch (...) {
			return;
		}
	}
}


const String RestRequest::findHeader(const String& name) const
{
	for (auto itr = _headers.begin(); itr != _headers.end(); ++itr) {
		if (itr->first == name) {
			return itr->second;
		}
	}

	return L"";
}


const String RestRequest::findQuery(const String& name) const
{
	for (auto itr = _querys.begin(); itr != _headers.end(); ++itr) {
		if (itr->first == name) {
			return itr->second;
		}
	}

	return L"";
}


void RestRequest::replyJson(const int32_t code, const String* pBody)
{
	if (!pBody) {
		_request.reply(web::http::status_codes::InternalError);
	}

	_request.reply(code, REST_STRING((*pBody)), U("application/json;"));
}


bool RestRequest::operator==(const RestRequest& req) const
{
	if (_uri == req._uri &&
		_relativeUri == req._relativeUri &&
		_path == req._path &&
		_ip == req._ip &&
		_contentType == req._contentType &&
		_body == req._body &&
		_queryStr == req._queryStr &&
		_headers == req._headers &&
		_querys == req._querys)
	{
		return true;
	}
	else {
		return false;
	}
}


bool RestRequest::operator!=(const RestRequest& req) const
{
	return !operator==(req);
}


RestRequest& RestRequest::operator=(const RestRequest& req)
{
	_uri = req._uri;
	_relativeUri = req._relativeUri;
	_path = req._path;
	_ip = req._ip;
	_contentType = req._contentType;
	_body = req._body;
	_queryStr = req._queryStr;
	_headers = req._headers;
	_querys = req._querys;

	return *this;
}

_namespace_server_end