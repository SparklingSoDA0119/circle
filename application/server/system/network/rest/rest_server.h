#ifndef _CIRCLE_SERVER_SYSTEM_NETWORK_REST_REST_SERVER_H_
#define _CIRCLE_SERVER_SYSTEM_NETWORK_REST_REST_SERVER_H_

#include "system/network/rest/rest_server_param.h"
#include "system/db/db_session_manager.h"
#include "system/network/rest/server/urls.h"
#include "system/network/rest/server/server_response.h"
#include "system/network/rest/rest_request.h"

#include <cpprest/http_listener.h>

_namespace_server_begin

class RestServer
{
public :
	RestServer();
	virtual ~RestServer();

public :
	int32_t initialize(const RestServerParam& param);

private :
	int32_t open();
	void handleAll(const web::http::http_request& req);

private :
	Response handleClub(const RestRequest* pReq);

private :
	bool _isOpen;
	RestServerParam _param;

private :
	web::http::experimental::listener::http_listener _listener;

private :
	DbSessionManager* _pDbSessManager;
	UrlSupport _urls;

};	// class RestServer

_namespace_server_end

#endif	// _CIRCLE_SERVER_SYSTEM_NETWORK_REST_REST_SERVER_H_