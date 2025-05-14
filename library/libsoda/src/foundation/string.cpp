#include "libsoda/foundation/string.h"
#include "libsoda/foundation/exception.h"

#include <vector>
#include <stdexcept>
#include <cstring>
#include <codecvt>
#include <sstream>

#if defined(_WIN64)
	#pragma warning(disable:4996) 
	#pragma warning(disable:4244)
#else
	#define _snwprintf  swprintf
#endif

_namespace_soda_begin

std::recursive_mutex g_defalutLocaleLock;
std::locale g_defaultLocale = std::locale("");
std::string g_defaultCharset = "";

void _wstring_vformat(std::wstring* result, const std::wstring& format, va_list args)
{
	static const std::wstring types(L"@diouxXDOUeEfFgGaACcSspn");

	result->clear();

	std::wstring tf;
	const size_t TS_N = 256;
	wchar_t ts[TS_N];
	std::unique_ptr<wchar_t> tss;
	std::wstring::const_iterator si = format.begin();
	std::wstring::const_iterator t, i = si;
	while (true) {
		i = std::find(i, format.end(), L'%');
		if (i == format.end()) {
			result->append(si, format.end());
			break;
		}

		if (*(i + 1) == L'%') {
			result->append(si, i + 1);
			i += 2;
			si = i;
			continue;
		}

		t = std::find_first_of(i, format.end(), types.begin(), types.end());
		if (t == format.end()) {
			std::wcerr << __FUNCTION__ << L"(): Unknown type " << "\"%" << *(i + 1) << "\"" << std::endl;
			abort();
			return;
		}

		result->append(si, i);

		if (*t == L'@') {
			Object* pObj = va_arg(args, Object*);
			if (pObj) {
				result->append(pObj->toString().to_wstring());
			}
			else {
				result->append(L"(null)");
			}
		}
		else {
			tf.assign(i, t + 1);
			int64_t dist = std::distance(i, t);
			if (*t == L'd' || *t == L'i' || *t == L'o' ||
				*t == L'u' || *t == L'x' || *t == L'X') {
				if (4 <= dist && *(t - 3) == 'I' && *(t - 2) == '6' && *(t - 1) == '4') {
#if !defined(_WIN64)
					tf.replace(tf.length() - 4, 3, L"ll");
#endif
					_snwprintf(ts, TS_N, tf.c_str(), va_arg(args, int64_t));
				}
				else if (3 <= dist && *(t - 2) == L'l' && *(t - 1) == L'l') {
#if defined(_WIN64)
					tf.replace(tf.length() - 3, 2, L"I64");
#endif
					_snwprintf(ts, TS_N, tf.c_str(), va_arg(args, int64_t));
				}
				else if (2 <= dist && *(t - 1) == L'l') {   // "%..ld"
					_snwprintf(ts, TS_N, tf.c_str(), va_arg(args, long));
				}
				else {                                      // "%..d"
					_snwprintf(ts, TS_N, tf.c_str(), va_arg(args, int));
				}
			}
			else if (*t == L'e' || *t == L'E' || *t == L'f' || *t == L'F' ||
				*t == L'g' || *t == L'G' || *t == L'a' || *t == L'A') {
				_snwprintf(ts, TS_N, tf.c_str(), va_arg(args, double));
			}
			else if (*t == L'c') {
#if defined(_WIN64)
				tf[tf.length() - 1] = L'C';
#else
				tf[tf.length() - 1] = L'c';
#endif
				_snwprintf(ts, TS_N, tf.c_str(), va_arg(args, int));
			}
			else if (*t == L'C') {
#if defined(_WIN64)
				tf[tf.length() - 1] = L'c';
#else
				tf[tf.length() - 1] = L'C';
#endif
				_snwprintf(ts, TS_N, tf.c_str(), va_arg(args, wint_t));
			}
			else if (*t == L's') {
#if defined(_WIN64)
				tf[tf.length() - 1] = L'S';
#else
				tf[tf.length() - 1] = L's';
#endif
				char* s = va_arg(args, char*);
				size_t tssn = std::strlen(s) + 1;
				if (TS_N < tssn) {
					tss.reset(new wchar_t[tssn]);
					_snwprintf(tss.get(), tssn, tf.c_str(), s);
				}
				else {
					_snwprintf(ts, TS_N, tf.c_str(), s);
				}
			}
			else if (*t == L'S') {
#if defined(_WIN64)
				tf[tf.length() - 1] = L's';
#else
				tf[tf.length() - 1] = L'S';
#endif
				wchar_t* ws = va_arg(args, wchar_t*);
				size_t tssn = std::wcslen(ws) + 1;
				if (TS_N < tssn) {
					tss.reset(new wchar_t[tssn]);
					_snwprintf(tss.get(), tssn, tf.c_str(), ws);
				}
				else {
					_snwprintf(ts, TS_N, tf.c_str(), ws);
				}
			}
			else if (*t == L'p') {
				_snwprintf(ts, TS_N, tf.c_str(), va_arg(args, void*));
			}

			if (tss.get()) {
				result->append(tss.get());
				tss.reset();
			}
			else {
				ts[TS_N - 1] = L'\0';
				result->append(ts);
			}
		}
		i = t + 1;
		si = i;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void String::makeUpper()
{
	std::lock_guard<std::recursive_mutex> al(defaultLocaleLock());
	makeUpper(defaultLocale());
}


void String::makeUpper(const std::locale& loc)
{
	std::wstring r;
	for (int i = 0; i < _wstring.size(); i++) {
		r.push_back(std::toupper<wchar_t>(_wstring[i], loc));
	}
	_wstring.swap(r);
}


void String::makeLower()
{
	std::lock_guard<std::recursive_mutex> al(defaultLocaleLock());
	makeLower(defaultLocale());
}


void String::makeLower(const std::locale& loc)
{
	std::wstring r;
	for (int i = 0; i < _wstring.size(); i++) {
		r.push_back(std::tolower<wchar_t>(_wstring[i], loc));
	}
	_wstring.swap(r);
}


void String::trimLeft()
{
	for (std::wstring::iterator i = _wstring.begin(); ; ++i) {
		if (i == _wstring.end()) {
			_wstring.clear();
			break;
		}
		else if (!std::isspace(*i, defaultLocale())) {
			_wstring.erase(_wstring.begin(), i);
			break;
		}
	}
}


void String::trimRight()
{
	for (std::wstring::iterator i = _wstring.end() - 1; ; --i) {
		if (!std::isspace(*i, defaultLocale())) {
			_wstring.erase(i + 1, _wstring.end());
			break;
		}
		else if (i == _wstring.begin()) {
			_wstring.clear();
			break;
		}
	}
}


void String::adjustToBytes(const size_t bytes)
{
	if (0 == bytes) {
		_wstring = L"";
	}
	else {
		std::string s = to_string();
		if (bytes < s.length()) {
			while (bytes < s.length()) {
				_wstring.resize(_wstring.length() - 1);
				s = to_string();
			}
			from_string(s);
		}
	}
}


void String::remove(const wchar_t wc)
{
	_wstring.erase(std::remove(_wstring.begin(), _wstring.end(), wc), _wstring.end());
}


void String::format(const wchar_t* format, ...)
{
	va_list args;
	va_start(args, format);
	_wstring_vformat(&_wstring, format, args);
	va_end(args);
}


void String::format(const String* format, ...)
{
	va_list args;
	va_start(args, format);
	_wstring_vformat(&_wstring, format->_wstring, args);
	va_end(args);
}


void String::vformat(const wchar_t* format, va_list args)
{
	_wstring_vformat(&_wstring, format, args);
}


void String::vformat(const String* format, va_list args)
{
	_wstring_vformat(&_wstring, format->_wstring, args);
}


void String::appendf(const wchar_t* format, ...)
{
	va_list args;
	va_start(args, format);
	std::wstring ws;
	_wstring_vformat(&ws, format, args);
	va_end(args);

	_wstring += ws;
}


void String::appendf(const String* format, ...)
{
	va_list args;
	va_start(args, format);
	std::wstring ws;
	_wstring_vformat(&ws, format->_wstring, args);
	va_end(args);

	_wstring += ws;
}


void String::appendvf(const wchar_t* format, va_list args)
{
	std::wstring ws;
	_wstring_vformat(&ws, format, args);

	_wstring += ws;
}


void String::appendvf(const String* format, va_list args)
{
	std::wstring ws;
	_wstring_vformat(&ws, format->_wstring, args);

	_wstring += ws;
}


#define THROW_RANGE_EXCEPTION(domain, index)											  \
    RangeException::info_s stInfo(0, static_cast<int32_t>(_wstring.length()) - 1, index); \
    throw RangeException(domain, stInfo)

#if !defined(_WIN64)
const size_t String::npos;
#endif

const wchar_t& String::at(const size_t index) const
{
	if (_wstring.length() <= index) {
		THROW_RANGE_EXCEPTION(L"String::at()", static_cast<int32_t>(index));
	}
	return _wstring[index];
}


int32_t String::compare(const size_t index, const size_t length, const String& s) const
{
	if (_wstring.length() <= index) {
		THROW_RANGE_EXCEPTION(L"String::compare()", static_cast<int32_t>(index));
	}
	return _wstring.compare(index, length, s._wstring);
}


bool String::caseInsensitiveCompare(const String& s) const
{
	struct lt_str_2 : public std::binary_function<std::wstring, std::wstring, bool>
	{
		bool operator()(const std::wstring& x, const std::wstring& y, const std::locale& loc) const
		{
			return std::lexicographical_compare(x.begin(),
				x.end(),
				y.begin(),
				y.end(),
				boost::is_iequal(loc));
		}
	};
	lt_str_2 comp;
	return comp(_wstring, s._wstring, defaultLocale());
}


size_t String::find(const String& s, const size_t index) const
{
	std::wstring::size_type i = _wstring.find(s._wstring, index);
	return (i == std::wstring::npos) ? String::npos : static_cast<uint32_t>(i);
}


size_t String::findFirstOf(const String& s, const size_t index) const
{
	std::wstring::size_type i = _wstring.find_first_of(s._wstring, index);
	return (i == std::wstring::npos) ? String::npos : static_cast<uint32_t>(i);
}


size_t String::findFirstNotOf(const String& s, const size_t index) const
{
	std::wstring::size_type i = _wstring.find_first_not_of(s._wstring, index);
	return (i == std::wstring::npos) ? String::npos : static_cast<uint32_t>(i);
}


size_t String::findLastOf(const String& s, const size_t index) const
{
	std::wstring::size_type i = _wstring.find_last_of(s._wstring, index);
	return (i == std::wstring::npos) ? String::npos : static_cast<uint32_t>(i);
}


size_t String::findLastNotOf(const String& s, const size_t index) const
{
	std::wstring::size_type i = _wstring.find_last_not_of(s._wstring, index);
	return (i == std::wstring::npos) ? String::npos : static_cast<uint32_t>(i);
}


size_t String::caseInsensitiveFind(const String& s) const
{
	std::wstring oriUpper;
	for (uint32_t i = 0; i < _wstring.size(); i++) {
		oriUpper.push_back(std::toupper<wchar_t>(_wstring[i], defaultLocale()));
	}

	std::wstring sUpper;
	for (uint32_t i = 0; i < s.length(); i++) {
		sUpper.push_back(std::toupper<wchar_t>(s[i], defaultLocale()));
	}
	std::wstring::size_type i = oriUpper.find_first_of(sUpper, 0);
	return (i == std::wstring::npos) ? String::npos : static_cast<uint32_t>(i);
}


size_t String::reverseFind(const String& s, const size_t index) const
{
	std::wstring::size_type pos = (index == String::npos) ?
		std::wstring::npos : static_cast<std::wstring::size_type>(index);
	std::wstring::size_type i = _wstring.rfind(s._wstring, pos);
	return (i == std::wstring::npos) ? String::npos : static_cast<const size_t>(i);
}


size_t String::count(const String& s) const
{
	size_t occurrences = 0;

	if (!isEmpty() && !s.isEmpty()) {
		const size_t len = s.length();

		size_t index = 0;
		while ((index = find(s, index)) != String::npos) {
			++occurrences;
			index += len;
		}
	}
	return occurrences;
}


String& String::replace(const size_t index, const size_t length, const String& s)
{
	if (_wstring.length() <= index) {
		THROW_RANGE_EXCEPTION(L"String::replace()", index);
	}
	_wstring.replace(index, length, s._wstring);
	return *this;
}


String& String::replace(const String& src, const String& dst, bool all)
{
	size_t srcLength = src.length();
	size_t dstLength = dst.length();
	size_t i = 0;
	do {
		i = find(src, i);
		if (i == npos) {
			break;
		}
		_wstring.replace(i, srcLength, dst._wstring);
		i += dstLength;
	} while (all);
	return *this;
}


String& String::insert(const size_t index, const String& s)
{
	if (_wstring.length() <= index) {
		THROW_RANGE_EXCEPTION(L"String::insert()", index);
	}
	_wstring.insert(index, s._wstring);
	return *this;
}


String String::substring(const size_t index, const size_t length) const
{
	if (_wstring.length() <= index) {
		THROW_RANGE_EXCEPTION(L"String::substring()", index);
	}
	else if (length != npos && _wstring.length() < index + length) {
		THROW_RANGE_EXCEPTION(L"String::substring()", index + length);
	}
	return _wstring.substr(index, length).c_str();
}


std::pair<String, String> String::devideBy(const String& token) const
{
	if (_wstring.empty()) {
		return std::pair<String, String>(token, L"");
	}

	size_t pos = _wstring.find(token.to_wstring());
	if (pos == std::wstring::npos) {
		return std::pair<String, String>(_wstring.c_str(), L"");
	}

	size_t ePos = token.length();
	if (pos + 1 >= _wstring.length()) {
		return std::pair<String, String>(_wstring.substr(0, pos).c_str(), L"");
	}

	return std::pair<String, String>(_wstring.substr(0, pos).c_str(), _wstring.substr(pos + ePos, _wstring.length() - ePos).c_str());
}


std::string String::to_string() const
{
	std::lock_guard<std::recursive_mutex> al(defaultLocaleLock());
	if (!defaultCharset().empty()) {
		return to_string(defaultCharset());
	}

	return to_string(defaultLocale());
}


std::string String::to_string(const std::locale& loc) const
{
#if defined(_WIN64)
	typedef std::codecvt<wchar_t, char, std::mbstate_t> codecvt_t;

	const std::wstring& ws = _wstring;
	codecvt_t const& codecvt = std::use_facet<codecvt_t>(loc);
	std::mbstate_t state = std::mbstate_t();
	std::vector<char> buffer((ws.length() + 1) * codecvt.max_length());
	const wchar_t* in = ws.c_str();
	char* out = &buffer[0];
	if (std::codecvt_base::error == codecvt.out(state, ws.c_str(), ws.c_str() + ws.length(), in, &buffer[0], &buffer[0] + buffer.size(), out)) {
		buffer[0] = '\0';
	}
	return std::string(&buffer[0]);
#else
	std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
	return myconv.to_bytes(_wstring);
#endif
}


std::string String::to_string(const std::string& charset) const
{
	/* not supported yet */
	return "";
}


void String::fromUtf8(const std::string& str)
{
	std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
	std::u16string utf16 = convert.from_bytes(str);
	_wstring = std::wstring(utf16.begin(), utf16.end());
	return;
}


std::string String::toUtf8Str() const
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	return converter.to_bytes(_wstring);
}


void String::from_string(const std::string& cs)
{
	std::lock_guard<std::recursive_mutex> al(defaultLocaleLock());
	if (!defaultCharset().empty()) {
		from_string(cs, defaultCharset());
	}
	else {
		from_string(cs, defaultLocale());
	}
}


void String::from_string(const char* cs)
{
	std::lock_guard<std::recursive_mutex> al(defaultLocaleLock());
	if (!defaultCharset().empty()) {
		from_string(cs, defaultCharset());
	}
	else {
		from_string(cs, defaultLocale());
	}
}


void String::from_string(const std::string& cs, const std::locale& loc)
{
#if defined(_WIN64)	
	typedef std::codecvt<wchar_t, char, std::mbstate_t> codecvt_t;

	const codecvt_t& codecvt = std::use_facet<codecvt_t>(loc);
	std::mbstate_t state = std::mbstate_t();
	std::vector<wchar_t> buffer(cs.length() + 1);
	const char* in = cs.c_str();
	wchar_t* out = &buffer[0];
	if (std::codecvt_base::error ==
		codecvt.in(state,
			cs.c_str(), cs.c_str() + cs.length(), in,
			&buffer[0], &buffer[0] + buffer.size(), out)) {
		buffer[0] = L'\0';
	}
	_wstring = &buffer[0];
#else
	std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
	_wstring = std::wstring(myconv.from_bytes(cs));
#endif
}


int8_t String::toInt8() const
{
	int value = 0;
	try {
		value = std::stoi(_wstring);
		if (value > 127 || value < -128) {
			throw BadCastException(L"String::toInt8()");
		}
	}
	catch (std::invalid_argument&) {
		throw BadCastException(L"String::toInt8()");
	}
	catch (std::out_of_range&) {
		throw BadCastException(L"String::toInt8()");
	}
	return value;
}


uint8_t String::toUint8() const
{
	unsigned long value = 0;
	try {
		value = std::stoul(_wstring);
		if (value > 255) {
			throw BadCastException(L"String::toUInt8()");
		}
	}
	catch (std::invalid_argument&) {
		throw BadCastException(L"String::toUInt8()");
	}
	catch (std::out_of_range&) {
		throw BadCastException(L"String::toUInt8()");
	}
	return value;
}


int16_t String::toInt16() const
{
	int value = 0;
	try {
		value = std::stoi(_wstring);
		if (value > 32767 || value < -32768) {
			throw BadCastException(L"String::toInt16()");
		}
	}
	catch (std::invalid_argument&) {
		throw BadCastException(L"String::toInt16()");
	}
	catch (std::out_of_range&) {
		throw BadCastException(L"String::toInt16()");
	}
	return value;
}


uint16_t String::toUint16() const
{
	unsigned long value = 0;
	try {
		value = std::stoul(_wstring);
		if (value > 65535) {
			throw BadCastException(L"String::toUInt16()");
		}
	}
	catch (std::invalid_argument&) {
		throw BadCastException(L"String::toUInt16()");
	}
	catch (std::out_of_range&) {
		throw BadCastException(L"String::toInt16()");
	}
	return value;
}


int32_t String::toInt32() const
{
	int value = 0;
	try {
		value = std::stoi(_wstring);
	}
	catch (std::invalid_argument&) {
		throw BadCastException(L"String::toInt32()");
	}
	catch (std::out_of_range&) {
		throw BadCastException(L"String::toInt32()");
	}
	return value;
}


uint32_t String::toUint32() const
{
	unsigned long value = 0;
	try {
		value = std::stoul(_wstring);
	}
	catch (std::invalid_argument&) {
		throw BadCastException(L"String::toUInt32()");
	}
	catch (std::out_of_range&) {
		throw BadCastException(L"String::toUInt32()");
	}
	return value;
}


int64_t String::toInt64() const
{
	int64_t value = 0;
	try {
		value = std::stoll(_wstring);
	}
	catch (std::invalid_argument&) {
		throw BadCastException(L"String::toInt64()");
	}
	catch (std::out_of_range&) {
		throw BadCastException(L"String::toInt64()");
	}
	return value;
}


uint64_t String::toUint64() const
{
	uint64_t value = 0;
	try {
		value = std::stoull(_wstring);
	}
	catch (std::invalid_argument&) {
		throw BadCastException(L"String::toUInt64()");
	}
	catch (std::out_of_range&) {
		throw BadCastException(L"String::toUInt64()");
	}
	return value;
}


float_t String::toFloat() const
{
	float_t value = 0;
	try {
		value = std::stof(_wstring);
	}
	catch (std::invalid_argument&) {
		throw BadCastException(L"String::toFloat32()");
	}
	catch (std::out_of_range&) {
		throw BadCastException(L"String::toFloat32()");
	}
	return value;
}


double_t String::toDouble() const
{
	double_t value = 0;
	try {
		value = std::stod(_wstring);
	}
	catch (std::invalid_argument&) {
		throw BadCastException(L"String::toDouble()");
	}
	catch (std::out_of_range&) {
		throw BadCastException(L"String::toDouble()");
	}
	return value;
}


uint64_t String::toHex() const
{
	std::wstringstream wss(_wstring);
	uint64_t value = 0;
	wss >> std::hex >> value;

	return value;
}
_namespace_soda_end