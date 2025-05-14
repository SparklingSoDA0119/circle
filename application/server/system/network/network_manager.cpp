#include "system/network/network_manager.h"

#include <libsoda/foundation/singlton.h>

_namespace_server_begin

NetworkManager::NetworkManager()
	: Looper(L"NetworkManager")
	, _isRun(false)
	, _pRestServer(nullptr)
{

}


NetworkManager::~NetworkManager()
{
	quit();
	join();
}


int32_t NetworkManager::initialize()
{
	int32_t ret = initRestServer();
	if (ret != 0) {
		return ret;
	}

	if (isAlive()) {
		resume();
	}
	else {
		run();
	}

	_isRun = true;

	return 0;
}


int32_t NetworkManager::initRestServer()
{
	if (_pRestServer) {
		printf("Info: RestServer had been created already.\n");
		return 0;
	}

	_pRestServer = soda::Singleton<RestServer>::get();
	if (!_pRestServer) {
		printf("Error: Can not create RestServer.\n");
		return 1;
	}

	RestServerParam param;
	param.setAddr(L"*");
	param.setPort(4817);
	param.setBaseUrl(L"circle/test/");
	param.setScheme(srv::NetScheme::HTTP);
	param.setTimeoutSec(15);

	int32_t ret = _pRestServer->initialize(param);
	if (ret != 0) {
		printf("Error: Can not initialize RestServer\n");
		return ret;
	}

	return 0;
}


void NetworkManager::looperFunc()
{
	if (_isRun) {

	}

	Thread::sleep(30);
}

_namespace_server_end