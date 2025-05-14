#ifndef _LIBSODA_DOCUMENT_JSON_JSON_H_
#define _LIBSODA_DOCUMENT_JSON_JSON_H_

#include "libsoda/foundation/string.h"

#if defined(_WIN64)
	#pragma warning(disable: 4244)
#endif

#define RAPIDJSON_HAS_STDSTRING		1

#include <rapidjson/document.h>

#include <vector>
#include <mutex>
#include <algorithm>
#include <map>
#include <fstream>
#include <type_traits>

typedef rapidjson::GenericDocument<rapidjson::UTF16<>>	JsonDocW;
typedef rapidjson::GenericValue<rapidjson::UTF16<>>		JsonValueW;

_namespace_soda_begin

class Json : public Object
{
public :
	Json();
	Json(const Json& json);

	virtual ~Json();

public :
	int32_t parse(const String& jsonStr);
	virtual int32_t parseMember() { return 0; }
	virtual void update() {}

public :
	int32_t read(const String& fname);
	int32_t write(const String& fname, const bool useEnter = true);

public :
	void clear();
	bool isExist(const String& name) const;

public :
	bool get(const String& name, bool* pValue);
	bool get(const String& name, int32_t* pValue);
	bool get(const String& name, uint32_t* pValue);
	bool get(const String& name, int64_t* pValue);
	bool get(const String& name, uint64_t* pValue);
	bool get(const String& name, float_t* pValue);
	bool get(const String& name, double_t* pValue);
	bool get(const String& name, String* pValue);

	void set(const String& name, const bool value);
	void set(const String& name, const int32_t value);
	void set(const String& name, const uint32_t value);
	void set(const String& name, const int64_t value);
	void set(const String& name, const uint64_t value);
	void set(const String& name, const float_t value);
	void set(const String& name, const double_t value);
	void set(const String& name, const String& value);

	bool push(const String& name, const bool value);
	bool push(const String& name, const int32_t value);
	bool push(const String& name, const uint32_t value);
	bool push(const String& name, const int64_t value);
	bool push(const String& name, const uint64_t value);
	bool push(const String& name, const float_t value);
	bool push(const String& name, const double_t value);
	bool push(const String& name, const String& value);

public :
	bool makeArray(const String& name);

private :
	JsonValueW makeValue(const String& str);

public :
	Json& operator=(const Json& json);

public :
	const String toString(const bool useEnter = false) const;

private :
	std::shared_ptr<JsonDocW> _pDoc;
};	// class Json

typedef std::shared_ptr<Json> JsonPtr;

_namespace_soda_end

#endif	// _LIBSODA_DOCUMENT_JSON_JSON_H_