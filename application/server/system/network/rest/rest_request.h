#ifndef _CIRCLE_SERVER_SYSTEM_NETWORK_REST_REST_REQUEST_H_
#define _CIRCLE_SERVER_SYSTEM_NETWORK_REST_REST_REQUEST_H_

#include "system/network/rest/rest_define.h"

#include <cpprest/http_msg.h>

_namespace_server_begin

class RestRequest
{
public :
	RestRequest();
	RestRequest(const RestRequest& req);
	RestRequest(const web::http::http_request& req);

	virtual ~RestRequest();

private :
	void parseQuery();

public :
	const String& uri() const;
	const String& relativeUri() const;
	const String& path() const;
	const String& ip() const;
	const String& contentType() const;
	const String& body() const;
	const String& queryStr() const;
	Method method() const;
	const std::map<String, String>& headers() const;
	const std::map<String, String>& querys() const;

public :
	const String findHeader(const String& name) const;
	const String findQuery(const String& name) const;

public :
	void replyJson(const int32_t code, const String* body);

public :
	bool operator==(const RestRequest& req) const;
	bool operator!=(const RestRequest& req) const;

	RestRequest& operator=(const RestRequest& req);

private :
	String _uri;
	String _relativeUri;
	String _path;
	String _ip;
	String _contentType;
	String _body;
	String _queryStr;
	Method _eMethod;
	std::map<String, String> _headers;
	std::map<String, String> _querys;

	web::http::http_request _request;
};	// class RestRequest

inline const String& RestRequest::uri() const { return _uri; }

inline const String& RestRequest::relativeUri() const { return _relativeUri; }

inline const String& RestRequest::path() const { return _path; }

inline const String& RestRequest::ip() const { return _ip; }

inline const String& RestRequest::contentType() const { return _contentType; }

inline const String& RestRequest::body() const { return _body; }

inline const String& RestRequest::queryStr() const { return _queryStr; }

inline Method RestRequest::method() const { return _eMethod; }

inline const std::map<String, String>& RestRequest::headers() const { return _headers; }

inline const std::map<String, String>& RestRequest::querys() const { return _querys; }




_namespace_server_end

#endif	// _CIRCLE_SERVER_SYSTEM_NETWORK_REST_REST_REQUEST_H_