#ifndef _CIRCLE_SERVER_SYSTEM_SYSTEM_H_
#define _CIRCLE_SERVER_SYSTEM_SYSTEM_H_

#include "system/db/db_session_manager.h"
#include "system/network/network_manager.h"

#include <libsoda/foundation/singlton.h>

_namespace_server_begin

class System : public soda::Looper
{
public :
	System();
	virtual ~System();

public :
	int32_t initialize();

private :
	int32_t initDbSessionManager();
	int32_t initNetworkManager();

protected :
	void looperFunc() override;

private :
	bool _isRun;

private :
	DbSessionManager* _pDbSessionManager;
	NetworkManager* _pNetworkManager;
};	// class System

_namespace_server_end

#endif // _CIRCLE_SERVER_SYSTEM_SYSTEM_H_