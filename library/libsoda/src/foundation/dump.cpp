#include "libsoda/foundation/dump.h"
#include "libsoda/foundation/string.h"

#if defined(_USE_DUMP)

_namespace_soda_begin

#if defined(_WIN64)
	int32_t WINAPI soda_dump(void* p, const wchar_t* fname);
#else
	int32_t soda_dump(void* p, const wchar* fname)
	{
		return 0;
	}
#endif

#if defined(_WIN64)
	int32_t WINAPI SODA_DUMP(void* p, const wchar_t* fname)
#else
	int32_t SODA_DUMP(void* p, const wchar* fname)
#endif
	{
		String s(fname ? fname : L"");
		String unavailable(L"\\/:*?\"<>|");

		uint32_t i = 0;
		while ((i = s.findFirstOf(unavailable, i)) != String::npos) {
			s[i++] = '_';
		}

		return soda_dump(p, s.cString());
	}

_namespace_soda_end

#endif	// _USE_DUMP