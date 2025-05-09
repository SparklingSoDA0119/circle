#ifndef _CIRCLE_SERVER_SYSTEM_NETWORK_NETWORK_MANAGER_H_
#define _CIRCLE_SERVER_SYSTEM_NETWORK_NETWORK_MANAGER_H_

#include "define.h"

#include <libsoda/foundation/looper.h>

_namespace_server_begin

class NetworkManager : public soda::Looper
{
public :
	NetworkManager();
	virtual ~NetworkManager();

public :
	int32_t initialize();

protected :
	void looperFunc() override;

private :
	bool _isRun;
};	// class NetworkManager

_namespace_server_end

#endif	// _CIRCLE_SERVER_SYSTEM_NETWORK_NETWORK_MANAGER_H_