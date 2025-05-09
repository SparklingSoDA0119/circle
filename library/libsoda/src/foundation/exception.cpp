#include "libsoda/foundation/exception.h"

_namespace_soda_begin

Exception::Exception()
	: _domain()
	, _code(0)
	, _pInfo(nullptr)
{

}


Exception::Exception(const String& domain, const int32_t code, const void* pInfo)
	: _domain(domain)
	, _code(code)
	, _pInfo(pInfo)
{

}


Exception::~Exception()
{

}


const String Exception::toString() const
{
	String s;
	s.format(L"%s, %d, 0x%x", _domain.cString(), _code, _pInfo);
	
	return String(s);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

RangeException::RangeException(const String& domain, const info_s& stInfo)
	: Exception(domain)
	, _stRangeInfo(stInfo)
{

}


RangeException::~RangeException()
{

}


const String RangeException::toString() const
{
	if (info()) {
		const info_s* pInfo = reinterpret_cast<const info_s*>(info());
		String s;
		s.format(L"%s, %d in [%d, %d]", domain().cString(), pInfo->pos, pInfo->begin, pInfo->end);
		return s;
	}
	else {
		return Exception::toString();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

BadAllocException::BadAllocException(const String& domain, const uint32_t allocSize)
	: Exception(domain)
	, _allocSize(allocSize)
{

}


BadAllocException::~BadAllocException()
{

}

const String BadAllocException::toString() const
{
	if (info()) {
		uint32_t allocSize = *reinterpret_cast<const uint32_t*>(info());
		String s;
		s.format(L"%s, allocSize: %u", domain().cString(), allocSize);
		return String(s);
	}
	else {
		return Exception::toString();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

BadCastException::BadCastException(const String& domain, const int32_t code, const void* pInfo)
	: Exception(domain, code, pInfo)
{

}


BadCastException::~BadCastException()
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

_namespace_soda_end