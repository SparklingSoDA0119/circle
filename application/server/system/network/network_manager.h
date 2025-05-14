#ifndef _CIRCLE_SERVER_SYSTEM_NETWORK_NETWORK_MANAGER_H_
#define _CIRCLE_SERVER_SYSTEM_NETWORK_NETWORK_MANAGER_H_

#include "system/network/rest/rest_server.h"

#include <libsoda/foundation/looper.h>

_namespace_server_begin

class NetworkManager : public soda::Looper
{
public :
	NetworkManager();
	virtual ~NetworkManager();

public :
	int32_t initialize();

private :
	int32_t initRestServer();

protected :
	void looperFunc() override;

private :
	bool _isRun;

private :
	RestServer* _pRestServer;
};	// class NetworkManager

_namespace_server_end

#endif	// _CIRCLE_SERVER_SYSTEM_NETWORK_NETWORK_MANAGER_H_