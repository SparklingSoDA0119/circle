#ifndef _LIBSODA_FOUNDATION_EXCEPTION_H_
#define _LIBSODA_FOUNDATION_EXCEPTION_H_

#include "libsoda/foundation/string.h"

_namespace_soda_begin

class Exception : public Object
{
public :
	Exception();
	Exception(const String& domain, const int32_t code = 0, const void* pInfo = nullptr);
	
	virtual ~Exception();

public :
	const String& domain() const;
	int32_t code() const;
	const void* info() const;

public :
	void setDomain(const String& domain);
	void setCode(const int32_t code);
	void setInfo(const void* pInfo);

public :
	const String toString() const override;

private :
	String _domain;
	int32_t _code;
	const void* _pInfo;
};	// class Exception

inline const String& Exception::domain() const { return _domain; }

inline int32_t Exception::code() const { return _code; }

inline const void* Exception::info() const { return _pInfo; }


inline void Exception::setDomain(const String& domain) { _domain = domain; }

inline void Exception::setCode(const int32_t code) { _code = code; }

inline void Exception::setInfo(const void* pInfo) { _pInfo = pInfo; }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

class RangeException : public Exception
{
public :
	struct info_s {
		size_t begin;
		size_t end;
		size_t pos;

		info_s() : begin(0), end(0), pos(0) {}
		info_s(const size_t b, const size_t e, const size_t p) : begin(b), end(e), pos(p) {}
	};

public :
	RangeException(const String& domain, const info_s& stInfo = info_s());
	
	virtual ~RangeException();

public :
	const String toString() const override;

protected :
	info_s _stRangeInfo;
};	// class RangeException

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

class BadAllocException : public Exception
{
public :
	BadAllocException(const String& domain, const uint32_t allocSize = 0);
	virtual ~BadAllocException();

public :
	const String toString() const override;

protected :
	uint32_t _allocSize;
};	// class BadAllocException

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

class BadCastException : public Exception
{
public :
	BadCastException(const String& domain, const int32_t code = 0, const void* pInfo = nullptr);
	virtual ~BadCastException();
};	// class BadCastException

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

_namespace_soda_end

#endif	// _LIBSODA_FOUNDATION_EXCEPTION_H_