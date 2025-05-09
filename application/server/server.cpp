#include "system/system.h"

#if defined(_WIN64)
	#if defined(_DEBUG)
		#pragma comment(lib, "libsodad.lib")
	#else	// release
	#endif
#endif

std::unique_ptr<cc::srv::System> _pSrvSystem = nullptr;

int32_t main(int32_t argc, char* arvg[])
{
	printf("<%s(%d)> ** Project 'Circle Server' start now..\n", __FUNCTION__, __LINE__);

	_pSrvSystem = std::make_unique<cc::srv::System>();
	if (!_pSrvSystem) {
		printf("<%s(%d)> ** Error: Can not create Server System.\n", __FUNCTION__, __LINE__);
		return 1;
	}

	if (_pSrvSystem->initialize() != 0) {
		printf("<%s(%d)> ** Error: Can not initialize Server System.\n", __FUNCTION__, __LINE__);
		return 1;
	}

	_pSrvSystem->join();

	printf("<%s(%d)> ** Project 'Circle Server' finish..\n", __FUNCTION__, __LINE__);

	return 0;
}