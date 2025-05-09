#include "system/network/network_manager.h"

_namespace_server_begin

NetworkManager::NetworkManager()
	: Looper(L"NetworkManager")
	, _isRun(false)
{

}


NetworkManager::~NetworkManager()
{
	quit();
	join();
}


int32_t NetworkManager::initialize()
{
	if (isAlive()) {
		resume();
	}
	else {
		run();
	}

	_isRun = true;

	return 0;
}


void NetworkManager::looperFunc()
{
	if (_isRun) {

	}

	Thread::sleep(30);
}

_namespace_server_end