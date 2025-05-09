#include "system/system.h"

_namespace_server_begin

System::System()
	: Looper(L"ServerSystem")
	, _isRun(false)
	, _pNetworkManager(nullptr)
{

}


System::~System()
{
	quit();
	join();
}


int32_t System::initialize()
{
	printf("INFO: Try System initialize..\n");

	int32_t ret = 0;

	ret = initNetworkManager();
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
	printf("INFO: System initialize success.\n");
	return 0;
}


int32_t System::initNetworkManager()
{
	if (_pNetworkManager) {
		printf("Info: NetworkManager have been created already.\n");
		return 1;
	}

	_pNetworkManager = soda::Singleton<NetworkManager>::get();
	if (!_pNetworkManager) {
		printf("Error: NetworkManager create failed.\n");
		return 2;
	}

	int32_t ret = _pNetworkManager->initialize();
	if (ret != 0) {
		printf("Error: NetworkManager initialize failed.\n");
		return 3;
	}

	printf("Info: NetworkManager initialize success.\n");
	return 0;
}


void System::looperFunc()
{
	if (_isRun) {

	}

	Thread::sleep(30);
}

_namespace_server_end