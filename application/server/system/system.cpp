#include "system/system.h"

_namespace_server_begin

System::System()
	: Looper(L"ServerSystem")
	, _isRun(false)
{

}


System::~System()
{
	quit();
	join();
}


int32_t System::initialize()
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


void System::looperFunc()
{
	if (_isRun) {

	}

	Thread::sleep(30);
}

_namespace_server_end