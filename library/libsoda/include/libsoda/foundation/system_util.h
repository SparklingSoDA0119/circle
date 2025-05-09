#ifndef _LIBSODA_FOUNDATION_SYSTEM_UTIL_H_
#define _LIBSODA_FOUNDATION_SYSTEM_UTIL_H_

#include "libsoda/foundation/string.h"

#include <list>

_namespace_soda_util_begin

namespace file
{

}	// namespace file

namespace tick
{
	uint64_t currentTick();

	uint64_t diffTick(uint64_t t1, uint64_t t2);

	uint64_t diffFromCurrentTick(uint64_t tick);
}

_namespace_soda_util_end

#endif	// _LIBSODA_FOUNDATION_SYSTEM_UTIL_H_