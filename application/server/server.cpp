#include <libsoda/foundation/singlton.h>

#include "system/system.h"


#if defined(_WIN64)
	#if defined(_DEBUG)
		#pragma comment(lib, "libsodad.lib")
	#else	// release
	#endif
#endif

cc::srv::System* _pSrvSystem = nullptr;

int32_t main(int32_t argc, char* arvg[])
{
	printf("<%s(%d)> ** Project 'Circle Server' start now..\n", __FUNCTION__, __LINE__);

	_pSrvSystem = soda::Singleton<cc::srv::System>::get();
	if (!_pSrvSystem) {
		printf("Error: Can not create Server System..\n");
		return 0;
	}

	if (_pSrvSystem->initialize() != 0) {
		printf("<%s(%d)> ** Error: Can not initialize Server System.\n", __FUNCTION__, __LINE__);
		return 1;
	}

	_pSrvSystem->join();

	printf("<%s(%d)> ** Project 'Circle Server' finish..\n", __FUNCTION__, __LINE__);

	return 0;
}