#include <libsoda/foundation/singlton.h>

#include "system/system.h"


#if defined(_WIN64)
	#include <Windows.h>
	#include <timeapi.h>
	#pragma comment(lib, "winmm")
	#if defined(_DEBUG)
		#pragma comment(lib, "libsodad.lib")
	#else	// release
	#endif
#endif

cc::srv::System* _pSrvSystem = nullptr;

int32_t main(int32_t argc, char* arvg[])
{
	printf("Info: Project 'Circle Server' start now..\n");

#if defined(_WIN64)
	::timeBeginPeriod(1);
#endif

	_pSrvSystem = soda::Singleton<cc::srv::System>::get();
	if (!_pSrvSystem) {
		printf("Error: Can not create Server System..\n");
		return 0;
	}

	if (_pSrvSystem->initialize() != 0) {
		printf("Error: Can not initialize Server System.\n");
		return 1;
	}

	_pSrvSystem->join();

	printf("Info: Project 'Circle Server' finish..\n");
#if defined(_WIN64)
	::timeEndPeriod(1);
#endif
	return 0;
}