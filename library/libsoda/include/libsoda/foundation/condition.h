#ifndef _LIBSODA_FOUNDATION_CONDITION_H_
#define _LIBSODA_FOUNDATION_CONDITION_H_

#include "libsoda/foundation/object.h"

#if defined(__linux__)
	#include <pthread.h>
#endif

#include <mutex>

_namespace_soda_begin

class Condition : public Object
{
public :
#if defined(_WIN64)
	typedef void* Handle;
#else
	typedef pthread_cond_t Handle;
#endif

public :
	Condition();
	virtual ~Condition();

	Handle& handle();
	bool isSignaled() const;
	bool wait(const uint32_t timeout);
	void signal();
	void reset();

private :
	Handle _handle;
	bool _signaled;
	std::mutex _lock;
};	// class Condition

inline Condition::Handle& Condition::handle() { return _handle; }

inline bool Condition::isSignaled() const { return _signaled; }

_namespace_soda_end

#endif	// _LIBSODA_FOUNDATION_CONDITION_H_