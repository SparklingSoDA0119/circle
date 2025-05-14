#ifndef _CIRCLE_SERVER_SYSTEM_NETWORK_REST_REST_SERVER_PARAM_H_
#define _CIRCLE_SERVER_SYSTEM_NETWORK_REST_REST_SERVER_PARAM_H_

#include "define.h"

#include <cpprest/uri_builder.h>

_namespace_server_begin

enum class NetScheme
{
	Undefined = 0,
	HTTP,
	HTTPS,
};	// enum class NetScheme

static const String toString(const NetScheme eScheme);


class RestServerParam : public soda::Object
{
public :
	RestServerParam();
	RestServerParam(const RestServerParam& param);

	virtual ~RestServerParam();

public :
	const String& addr() const;
	uint32_t port() const;
	const String& baseUrl() const;
	uint32_t timeoutSec() const;
	NetScheme scheme() const;

	void setAddr(const String& addr);
	void setPort(const uint32_t port);
	void setBaseUrl(const String& url);
	void setTimeoutSec(const uint32_t sec);
	void setScheme(const NetScheme eScheme);

public :
	const web::uri_builder getUrlBuilder() const;

public :
	bool operator==(const RestServerParam& param) const;
	bool operator!=(const RestServerParam& param) const;

	RestServerParam& operator=(const RestServerParam& param);

public : 
	const String toString() const override;

private :
	String _addr;
	uint32_t _port;
	String _baseUrl;
	uint32_t _timeoutSec;
	NetScheme _eScheme;
};	// class RestServerParam

inline const String& RestServerParam::addr() const { return _addr; }

inline uint32_t RestServerParam::port() const { return _port; }

inline const String& RestServerParam::baseUrl() const { return _baseUrl; }

inline uint32_t RestServerParam::timeoutSec() const { return _timeoutSec; }

inline NetScheme RestServerParam::scheme() const { return _eScheme; }


inline void RestServerParam::setAddr(const String& addr) { _addr = addr; }

inline void RestServerParam::setPort(const uint32_t port) { _port = port; }

inline void RestServerParam::setBaseUrl(const String& url) { _baseUrl = url; }

inline void RestServerParam::setTimeoutSec(const uint32_t sec) { _timeoutSec = sec; }

inline void RestServerParam::setScheme(const NetScheme eScheme) { _eScheme = eScheme; }

_namespace_server_end

#endif	// _CIRCLE_SERVER_SYSTEM_NETWORK_REST_REST_SERVER_PARAM_H_