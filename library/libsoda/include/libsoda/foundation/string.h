#ifndef _LIBSODA_FOUNDATION_STRING_H_
#define _LIBSODA_FOUNDATION_STRING_H_

#include "libsoda/foundation/object.h"

#include <functional>
#include <algorithm>
#include <cstdarg>
#include <locale>
#include <mutex>

namespace boost {
	struct is_iequal {
		is_iequal(const std::locale& loc = std::locale()) : _loc(loc) {}

		template <typename T1, typename T2>
		bool operator()(const T1& arg1, const T2& arg2) const {
			return std::toupper<T1>(arg1, _loc) == std::toupper<T2>(arg2, _loc);
		}

	private :
		std::locale _loc;
	};
}	// namespace boost

_namespace_soda_begin

class String : public Object
{
public :
	static const size_t npos = std::wstring::npos;

public :
	static std::recursive_mutex& defaultLocaleLock()
	{
		static std::recursive_mutex s_defaultLocaleMutex;
		return s_defaultLocaleMutex;
	}

	static const std::locale& defaultLocale()
	{
		static std::locale s_defaultLocale(std::locale(""));

		std::lock_guard<std::recursive_mutex> am(defaultLocaleLock());
		return s_defaultLocale;
	}

	static void setDefaultLocale(const std::locale& loc)
	{
		std::lock_guard<std::recursive_mutex> am(defaultLocaleLock());
		const_cast<std::locale&>(defaultLocale()) = loc;
	}

	static const std::string& defaultCharset()
	{
		static std::string s_defaultCharset("");

		std::lock_guard<std::recursive_mutex> am(defaultLocaleLock());
		return s_defaultCharset;
	}

	static void setDefaultCharset(const std::string& charset)
	{
		std::lock_guard<std::recursive_mutex> am(defaultLocaleLock());
		const_cast<std::string&>(defaultCharset()) = charset;
	}

public:
	explicit String() {}

	String(const String& s) : _wstring(s._wstring) {}
	String(const String& s, const size_t index, const size_t length = npos);

	String(const wchar_t* wcs);
	String(const wchar_t* wcs, const size_t length);

	String(const size_t count, wchar_t ch) : _wstring(static_cast<size_t>(count), ch) {}

	String(const std::wstring& cs);

	String(const std::string& cs);
	String(const std::string& cs, const std::locale& loc);
	String(const std::string& cs, const std::string& charset);

	explicit String(const char* cs);
	explicit String(const char* cs, const std::locale& loc);
	explicit String(const char* cs, const std::string& charset);

	String(String&& s) noexcept = default;

	virtual ~String() { _wstring.clear(); }

public :
	std::shared_ptr<Object> clone() const override { return std::make_shared<String>(*this); }

public:
	bool isEmpty() const;
	size_t length() const;
	size_t capacity() const;
	void reserve(const size_t capacity);

public:
	void clear();
	void resize(const size_t size);
	void resize(const size_t size, wchar_t wc);
	void assign(const String& s);
	void append(const size_t count, wchar_t wc);
	void append(const String& s);

	String& operator=(const wchar_t* wc);
	String& operator=(const String& s);
	String& operator=(String&& s) noexcept;
	String& operator=(const char* s);

	String& operator+=(const String& s);
	String& operator+=(wchar_t wc);

	void swap(String& s);
	void makeUpper();
	void makeUpper(const std::locale& loc);
	void makeLower();
	void makeLower(const std::locale& loc);

	void trimLeft();
	void trimRight();
	void adjustToBytes(const size_t bytes);
	void remove(const wchar_t wc);

public:
	void format(const wchar_t* format, ...);
	void format(const String* format, ...);

	void vformat(const wchar_t* format, va_list args);
	void vformat(const String* format, va_list args);

	void appendf(const wchar_t* format, ...);
	void appendf(const String* format, ...);

	void appendvf(const wchar_t* format, va_list args);
	void appendvf(const String* format, va_list args);

public:
	const wchar_t& operator[](const size_t index) const;
	wchar_t& operator[](const size_t index);
	const wchar_t& at(const size_t index) const;
	wchar_t& at(const size_t index);

public:
	int32_t compare(const String& s) const;
	int32_t compare(const size_t index, const size_t length, const String& s) const;

	bool caseInsensitiveCompare(const String& s) const;

public:
	bool operator==(const String& s) const;
	bool operator!=(const String& s) const;
	bool operator<=(const String& s) const;
	bool operator< (const String& s) const;
	bool operator>=(const String& s) const;
	bool operator> (const String& s) const;

	bool operator==(const wchar_t* wcs) const;
	bool operator!=(const wchar_t* wcs) const;
	bool operator<=(const wchar_t* wcs) const;
	bool operator< (const wchar_t* wcs) const;
	bool operator>=(const wchar_t* wcs) const;
	bool operator> (const wchar_t* wcs) const;

	friend bool operator==(const wchar_t* wcs, const String& s);
	friend bool operator!=(const wchar_t* wcs, const String& s);
	friend bool operator<=(const wchar_t* wcs, const String& s);
	friend bool operator< (const wchar_t* wcs, const String& s);
	friend bool operator>=(const wchar_t* wcs, const String& s);
	friend bool operator> (const wchar_t* wcs, const String& s);

public:
	size_t find(const String& s, const size_t index = 0) const;
	size_t findFirstOf(const String& s, const size_t index = 0) const;
	size_t findFirstNotOf(const String& s, const size_t index = 0) const;
	size_t findLastOf(const String& s, const size_t index = npos) const;
	size_t findLastNotOf(const String& s, const size_t index = npos) const;

	size_t caseInsensitiveFind(const String& s) const;
	size_t reverseFind(const String& s, const size_t index = npos) const;

	size_t count(wchar_t ch) const;
	size_t count(const String& s) const;

	String& replace(size_t index, size_t length, const String& s);
	String& replace(const String& src, const String& dst, bool all = true);

	String& insert(size_t index, const String& s);
	String substring(size_t index, size_t length = npos) const;
	std::pair<String, String> devideBy(const String& token) const;

public:
	const wchar_t* cString() const;
	const std::wstring& to_wstring() const;
	void from_wstring(const std::wstring& ws);
	void fromUtf8(const std::string& str);

public:
	std::string to_string() const;
	std::string to_string(const std::locale& loc) const;
	std::string to_string(const std::string& charset) const;
	std::string toUtf8Str() const;

	void from_string(const std::string& cs);
	void from_string(const char* cs);
	void from_string(const std::string& cs, const std::locale& loc);
	void from_string(const std::string& cs, const std::string& charset) {}
	void from_string(const char* cs, const std::locale& loc);
	void from_string(const char* cs, const std::string& charset);

public:
#if defined(_WIN64)
	#if defined(_UNICODE)
		const TCHAR* to_LPCTSTR() const { return to_wstring().c_str(); }
	#else
		const TCHAR* to_LPCTSTR() const { return ""; }
	#endif
#endif

public:
	int8_t   toInt8()	const;
	uint8_t  toUint8()	const;
	int16_t  toInt16()	const;
	uint16_t toUint16() const;
	int32_t  toInt32()	const;
	uint32_t toUint32() const;
	int64_t  toInt64()	const;
	uint64_t toUint64() const;
	float_t  toFloat()  const;
	double_t toDouble() const;
	uint64_t toHex()	const;

public:
	virtual size_t hash() const { return std::hash<std::wstring>()(_wstring); }

public:
	const String toString() const override { return *this; }

private :
	std::wstring _wstring;
};	// class String

inline String::String(const String& s, const size_t index, const size_t length)
	: _wstring(s._wstring, static_cast<size_t>(index)
	, (npos == length) ? std::wstring::npos : static_cast<size_t>(length)) {
}

inline String::String(const wchar_t* wcs)
{
	try {
		_wstring = std::wstring(wcs);
	}
	catch (std::exception& e) {
		printf("<%s:%s(%d)> ** error : %s\n", __FILE__, __FUNCTION__, __LINE__, e.what());
		abort();
	}
}

inline String::String(const std::wstring& cs) { from_wstring(cs); }

inline String::String(const wchar_t* wcs, const size_t length) : _wstring(wcs, static_cast<size_t>(length)) {}

inline String::String(const std::string& cs) { from_string(cs); }

inline String::String(const std::string& cs, const std::locale& loc) { from_string(cs, loc); }

inline String::String(const std::string& cs, const std::string& charset) { from_string(cs, charset); }

inline String::String(const char* cs) { from_string(cs); }

inline String::String(const char* cs, const std::locale& loc) { from_string(cs, loc); }

inline String::String(const char* cs, const std::string& charset) { from_string(cs, charset); }

inline void String::reserve(const size_t capacity) { _wstring.reserve(static_cast<std::wstring::size_type>(capacity)); }

inline void String::resize(const size_t size) { _wstring.resize(static_cast<std::wstring::size_type>(size)); }

inline void String::resize(const size_t size, wchar_t wc) { _wstring.resize(static_cast<std::wstring::size_type>(size), wc); }

inline wchar_t& String::at(const size_t index) { return const_cast<wchar_t&>(static_cast<const String*>(this)->at(index)); }

inline void String::from_string(const char* cs, const std::locale& loc) {
	try {
		from_string(std::string(cs), loc);
	}
	catch (std::exception& e) {
		printf("<%s:%s(%d)> ** error : %s\n", __FILE__, __FUNCTION__, __LINE__, e.what());
		abort();
	}
}

inline void String::from_string(const char* cs, const std::string& charset) {
	try {
		from_string(std::string(cs), charset);
	}
	catch (std::exception& e) {
		printf("<%s:%s(%d)> ** error : %s\n", __FILE__, __FUNCTION__, __LINE__, e.what());
		abort();
	}
}

inline bool String::isEmpty() const { return _wstring.empty(); }

inline size_t String::length() const { return static_cast<size_t>(_wstring.length()); }

inline size_t String::capacity() const { return static_cast<size_t>(_wstring.capacity()); }

inline void String::clear() { _wstring.clear(); }

inline size_t String::count(wchar_t ch) const { return static_cast<size_t>(std::count(_wstring.begin(), _wstring.end(), ch)); }

inline void String::assign(const String& s) { _wstring.assign(s._wstring); }

inline void String::append(const size_t count, wchar_t wc) { _wstring.append(count, wc); }

inline void String::append(const String& s) { _wstring.append(s._wstring); }

inline String& String::operator=(const wchar_t* wc) { _wstring = std::wstring(wc); return *this; }

inline String& String::operator=(const String& s) { _wstring.assign(s._wstring); return *this; }

inline String& String::operator=(String&& s) noexcept { _wstring = std::move(s._wstring); return *this; }

inline String& String::operator=(const char* s) { from_string(s); return *this; }

inline String& String::operator+=(const String& s) { _wstring.append(s._wstring); return *this; }

inline String& String::operator+=(wchar_t wc) { _wstring += wc; return *this; }

inline void String::swap(String& s) { _wstring.swap(s._wstring); }

inline const wchar_t& String::operator[](const size_t index) const { return _wstring[index]; }

inline wchar_t& String::operator[](const size_t index) { return _wstring[index]; }

inline int32_t String::compare(const String& s) const { return _wstring.compare(s._wstring); }

inline bool String::operator==(const String& s)  const { return _wstring == s._wstring; }

inline bool String::operator!=(const String& s)  const { return _wstring != s._wstring; }

inline bool String::operator<=(const String& s)  const { return _wstring <= s._wstring; }

inline bool String::operator< (const String& s)  const { return _wstring < s._wstring; }

inline bool String::operator>=(const String& s)  const { return _wstring >= s._wstring; }

inline bool String::operator> (const String& s)  const { return _wstring > s._wstring; }

inline bool String::operator==(const wchar_t* wcs) const { return _wstring == wcs; }

inline bool String::operator!=(const wchar_t* wcs) const { return _wstring != wcs; }

inline bool String::operator<=(const wchar_t* wcs) const { return _wstring <= wcs; }

inline bool String::operator< (const wchar_t* wcs) const { return _wstring < wcs; }

inline bool String::operator>=(const wchar_t* wcs) const { return _wstring >= wcs; }

inline bool String::operator> (const wchar_t* wcs) const { return _wstring > wcs; }

inline bool operator==(const wchar_t* wcs, const String& s) { return wcs == s._wstring; }

inline bool operator!=(const wchar_t* wcs, const String& s) { return wcs != s._wstring; }

inline bool operator<=(const wchar_t* wcs, const String& s) { return wcs <= s._wstring; }

inline bool operator< (const wchar_t* wcs, const String& s) { return wcs < s._wstring; }

inline bool operator>=(const wchar_t* wcs, const String& s) { return wcs >= s._wstring; }

inline bool operator> (const wchar_t* wcs, const String& s) { return wcs > s._wstring; }

inline String operator+(const String& s1, const String& s2) { return String(s1) += s2; }

inline String operator+(const wchar_t* wcs, const String& s) { return String(wcs) += s; }

inline String operator+(wchar_t wc, const String& s) { return String(1, wc) += s; }

inline String operator+(const String& s, const wchar_t* wcs) { return String(s) += wcs; }

inline String operator+(const String& s, wchar_t wc) { return String(s) += wc; }

inline const wchar_t* String::cString() const { return _wstring.c_str(); }

inline const std::wstring& String::to_wstring() const { return _wstring; }

inline void String::from_wstring(const std::wstring& ws) { _wstring = ws; }

_namespace_soda_end

namespace std
{
	template <>
	struct hash<soda::String>
	{
		size_t operator()(const soda::String& s) const { return s.hash(); }
	};
}

#endif	// _LIBSODA_FOUNDATION_STRING_H_