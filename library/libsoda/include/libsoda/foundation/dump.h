#ifndef _LIBSODA_FOUNDATION_DUMP_H_
#define _LIBSODA_FOUNDATION_DUMP_H_

#include "libsoda/foundation/type.h"

#if !defined(_DONT_USE_DUMP)
	#define _USE_DUMP
#endif

_namespace_soda_begin

#if defined(_USE_DUMP)
	#if defined(_WIN64)
		int32_t WINAPI SODA_DUMP(void* p, const wchar_t* fname);
	#else
		int32_t SODA_DUMP(void* p, const wchar_t* fname);
	#endif
#else
	#define SODA_DUMP(p, fname)	// do nothing
#endif
_namespace_soda_end


#endif	// _LIBSODA_FOUNDATION_DUMP_H_