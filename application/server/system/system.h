#include "define.h"

#include <libsoda/foundation/looper.h>

_namespace_server_begin

class System : public soda::Looper
{
public :
	System();
	virtual ~System();

public :
	int32_t initialize();

protected :
	void looperFunc() override;

private :
	bool _isRun;
};	// class System

_namespace_server_end