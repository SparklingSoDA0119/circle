#ifndef _CIRCLE_SERVER_SYSTEM_NETWORK_REST_SERVER_URLS_H_
#define _CIRCLE_SERVER_SYSTEM_NETWORK_REST_SERVER_URLS_H_

#include "system/network/rest/rest_define.h"

_namespace_server_begin

class Url
{
public :
	Url(const String& url = L"", const std::vector<Method>& mets = {});
	virtual ~Url();

public :
	const String& url() const;
	const std::vector<Method>& methods() const;

public :
	bool isMatched(const String& url);
	bool isMatched(const String& url, const Method eMethod);

private :
	String _url;
	std::vector<Method> _mets;
};	// class Url;

inline const String& Url::url() const { return _url; }

inline const std::vector<Method>& Url::methods() const { return _mets; }


class UrlSupport
{
public :
	UrlSupport();
	virtual ~UrlSupport();

private :
	void makeSupportUrl();

public :
	bool isSupport(const String& url, const Method eMet);

private :
	std::vector<Url> _supportedUrl;
};	// class UrlSupport

_namespace_server_end

#endif	// _CIRCLE_SERVER_SYSTEM_NETWORK_REST_SERVER_URLS_H_