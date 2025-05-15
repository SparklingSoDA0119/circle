#include "system/system.h"

_namespace_server_begin

System::System()
	: Looper(L"ServerSystem")
	, _isRun(false)
	, _pDbSessionManager(nullptr)
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

	ret = initDbSessionManager();
	if (ret != 0) {
		return ret;
	}

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


int32_t System::initDbSessionManager()
{
	if (_pDbSessionManager) {
		printf("Info: DbSessionManager had been created already.\n");
		return 1;
	}

	_pDbSessionManager = soda::Singleton<DbSessionManager>::get();
	if (!_pDbSessionManager) {
		printf("Error: DbSessionManager create failed.\n");
		return 2;
	}

	DbSessionParam param;
	param.setHost(L"127.0.0.1");
	param.setId(L"root");
	param.setPw(L"soda");
	param.setPort(13306);
	param.setDbName(L"circle_test");

	int32_t ret = _pDbSessionManager->initialize(param);
	if (ret != 0) {
		printf("Error: DbSessionManager initialize failed.\n");
		return 3;
	}

	printf("Info: DbSessionManager initialize success.\n");
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