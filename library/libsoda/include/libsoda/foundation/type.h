#ifndef _LIBSODA_FOUNDATION_TYPE_H_
#define _LIBSODA_FOUNDATION_TYPE_H_

#include <iostream>
#include <vector>
#include <memory>

#define _namespace_soda_begin	namespace soda {
#define _namespace_soda_end		}

#define _namespace_soda_util_begin	_namespace_soda_begin namespace util {
#define _namespace_soda_util_end    } _namespace_soda_end

#define _namespace_soda_data_begin	_namespace_soda_begin namespace data {
#define _namespace_soda_data_end    } _namespace_soda_end

#if defined(_WIN64)
	#include <ctime>
#endif

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
	#pragma once
	#pragma warning(disable : 4819) // cp949 warning
#endif

#if defined(_MSC_VER) && (_MSC_VER >= 1700)
	#if defined(WINAPI_FAMILY) && (WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP) && !defined(_WIN_PHONE)
		#define _WIN_PHONE 1
	#endif // _WIN_PHONE
#endif

#if defined(_WIN32)
	typedef      __time32_t  time_t32;
	typedef      __time64_t  time_t64;
#else
	typedef         int32_t   time_t32;
	typedef         int64_t   time_t64;
#endif


inline uint16_t makeUint16(const uint8_t h, const uint8_t l) { return (static_cast<uint16_t>(h) << 8) | l; }
inline uint8_t  highUint8(const uint16_t i)					 { return static_cast<uint8_t>((i >> 8) & 0xFF); }
inline uint8_t  lowUint8(const uint16_t i)					 { return static_cast<uint8_t>(i); }

inline uint32_t makeUint32(const uint16_t h, const uint16_t l) { return (static_cast<uint32_t>(h) << 16) | l; }
inline uint16_t highUint16(const uint32_t i)				   { return static_cast<uint16_t>((i >> 16) & 0xFFFF); }
inline uint16_t lowUint16(const uint32_t i)					   { return static_cast<uint16_t>(i); }

inline uint64_t makeUint64(const uint32_t h, const uint32_t l) { return (static_cast<uint64_t>(h) << 32) | l; }
inline uint32_t highUint32(const uint64_t i)			       { return static_cast<uint32_t>((i >> 32) & 0xFFFFFFFF); }
inline uint32_t lowUint32(const uint64_t i)					   { return static_cast<uint32_t>(i); }

template<typename T>
inline T absDiff(const T& a, const T& b) { return (a < b) ? (b - a) : (a - b); }

typedef std::vector<unsigned char> Binary;
typedef std::shared_ptr<Binary> BinaryPtr;

#if defined(_WIN64)
#define _DEPRECATED  __declspec(deprecated)
#else   // gcc
#define _DEPRECATED  __attribute__((__deprecated__))
#endif /* __GNUC__ */


#if defined(_WIN64)
	#ifndef NOMINMAX
		#define NOMINMAX
	#endif
	#ifdef min
		#undef min
	#endif
	#ifdef max
		#undef max
	#endif

	#ifndef DECLSPEC_IMPORT
		#if (defined(_M_IX86) || defined(_M_IA64) || defined(_M_AMD64) || defined(_M_ARM)) && !defined(MIDL_PASS)
			#define DECLSPEC_IMPORT __declspec(dllimport)
		#else
			#define DECLSPEC_IMPORT
		#endif
	#endif  // DECLSPEC_IMPORT

	#ifndef WINBASEAPI
		#if !defined(_KERNEL32_)
			#define WINBASEAPI  DECLSPEC_IMPORT
		#else
			#define WINBASEAPI
		#endif
	#endif  // WINBASEAPI

	#ifndef WINUSERAPI
		#if !defined(_USER32_)
			#define WINUSERAPI  DECLSPEC_IMPORT
		#else
			#define WINUSERAPI
		#endif
	#endif  // WINUSERAPI

	#ifndef WINAPI
		#if (_MSC_VER >= 800) || defined(_STDCALL_SUPPORTED)
			#define WINAPI  __stdcall
		#else
			#define WINAPI
		#endif
	#endif  // WINAPI

	#ifndef FALSE
		#define FALSE   0
	#endif
	
	#ifndef TRUE
		#define TRUE    1
	#endif

	#if defined(UNICODE)
		#if !defined(_TCHAR_DEFINED)
			typedef wchar_t TCHAR;
			#define _TCHAR_DEFINED
		#endif

		#define __TEXT(quote) L##quote
	#else
		#if !defined(_TCHAR_DEFINED)
			typedef char TCHAR;
			#define _TCHAR_DEFINED
		#endif
			#define __TEXT(quote) quote
		#endif
#endif

#if defined(_MSC_VER)
	#define PACK_BEGIN  __pragma(pack(push, 1))
	#define PACK_END    __pragma(pack(pop))
#elif defined(__APPLE__)
	#define PACK_BEGIN
	#define PACK_END    __attribute__((packed))
#elif defined (__GNUC__) || defined(ANDROID)
	#if defined (__arm)
		#define PACK_BEGIN  __packed
		#define PACK_END
	#else
		#define PACK_BEGIN
		#define PACK_END    __attribute__((packed))
	#endif
#endif

#define _M_reset(Pointer)	if (Pointer) { Pointer.reset(); Pointer = nullptr; }

#endif	// _LIBSODA_FOUNDATION_TYPE_H_