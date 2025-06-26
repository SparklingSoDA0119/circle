#include "system/network/rest/server/urls.h"

_namespace_server_begin

Url::Url(const String& url, const std::vector<Method>& mets)
	: _url(url)
	, _mets(mets)
{

}


Url::~Url()
{

}


bool Url::isMatched(const String& url)
{
	return _url == url;
}


bool Url::isMatched(const String& url, const Method eMethod)
{
	if (!isMatched(url)) {
		return false;
	}

	for (auto itr = _mets.begin(); itr != _mets.end(); ++itr) {
		if (*itr == eMethod) {
			return true;
		}
	}

	return false;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UrlSupport::UrlSupport()
	: _supportedUrl{}
{
	makeSupportUrl();
}


UrlSupport::~UrlSupport()
{

}


void UrlSupport::makeSupportUrl()
{
	_supportedUrl.push_back(Url(L"test/auth", { Method::Get, Method::Post }));
}


bool UrlSupport::isSupport(const String& url, const Method eMet)
{
	for (auto itr = _supportedUrl.begin(); itr != _supportedUrl.end(); ++itr) {
		if (itr->isMatched(url, eMet)) {
			return true;
		}
	}

	return false;
}


_namespace_server_end