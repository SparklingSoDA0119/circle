#include "libsoda/document/json/json.h"

#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/ostreamwrapper.h>

_namespace_soda_begin

typedef rapidjson::GenericStringBuffer<rapidjson::UTF16<>>								   JsonStringBufferW;
typedef rapidjson::PrettyWriter<JsonStringBufferW, rapidjson::UTF16<>, rapidjson::UTF16<>> PrettyWriterW;
typedef rapidjson::Writer<JsonStringBufferW, rapidjson::UTF16<>, rapidjson::UTF16<>>	   WriterW;

Json::Json()
	: _pDoc(nullptr)
{
	_pDoc = std::make_shared<JsonDocW>();
}


Json::Json(const Json& json)
{
	operator=(json);
}


Json::~Json()
{
	_M_reset(_pDoc)
}


int32_t Json::parse(const String& jsonStr)
{
	clear();

	_pDoc->Parse(jsonStr.to_wstring());
	if (_pDoc->HasParseError()) {
		return 1;
	}

	return parseMember();
}


int32_t Json::read(const String& fname)
{
	if (fname.isEmpty()) {
		return 1;
	}

	std::ifstream ifs(fname.to_wstring());

	if (!ifs.is_open()) {
		return 2;
	}

	std::string s;
	{
		ifs.seekg(0, std::ios::end);
		size_t ifsSize = ifs.tellg();

		if (ifsSize == 0) {
			return 3;
		}

		ifs.seekg(0, std::ios::beg);


		s.resize(ifsSize);

		ifs.read(&s[0], ifsSize);
		ifs.seekg(0, std::ios::beg);

		for (auto itr = s.begin(); itr != s.end(); ++itr) {
			if (*itr == ' ' || *itr == '\0') {
				continue;
			}

			if (*itr == '{') {
				break;
			}
			else {
				return 4;
			}
		}
	}
	ifs.close();

	String readStr;
	readStr.fromUtf8(s);

	return parse(s);
}


int32_t Json::write(const String& fname, const bool useEnter)
{
	if (fname.isEmpty()) {
		return 1;
	}

	std::ofstream ofs(fname.to_wstring());
	if (!ofs.is_open()) {
		return 1;
	}

	update();

	rapidjson::OStreamWrapper osw(ofs);
	if (useEnter) {
		rapidjson::PrettyWriter<rapidjson::OStreamWrapper, rapidjson::UTF16<>, rapidjson::UTF8<>> writer(osw);
		_pDoc->Accept(writer);
	}
	else {
		rapidjson::Writer<rapidjson::OStreamWrapper, rapidjson::UTF16<>, rapidjson::UTF8<>> writer(osw);
		_pDoc->Accept(writer);
	}

	return 0;
}


void Json::clear()
{
	_M_reset(_pDoc);
	_pDoc = std::make_shared<JsonDocW>();
}


bool Json::isExist(const String& name) const
{
	if (!_pDoc->IsObject()) {
		return false;
	}

	return _pDoc->HasMember(name.to_wstring());
}

#define _M_is_exist		if (!isExist(name)) { return false; }

#define _M_get_value(Type)								\
	auto itr = _pDoc->FindMember(name.to_wstring());	\
	if (itr != _pDoc->MemberEnd()) {					\
		if (itr->value.Is##Type()) {					\
			*pValue = itr->value.Get##Type();			\
			return true;								\
		}												\
	}													\
	return false;

#define _M_get_func(DataType, JsonType)							\
	bool Json::get(const String& name, DataType* pValue)	{	\
		_M_is_exist _M_get_value(JsonType)	}

#define _M_set_func(DataType, JsonType)							\
	void Json::set(const String& name, const DataType value) {	\
		if (isExist(name)) {									\
			auto itr = _pDoc->FindMember(name.to_wstring());	\
			itr->value.Set##JsonType(value);					\
		}	\
		else {		\
			_pDoc->AddMember(makeValue(name), JsonValueW(value), _pDoc->GetAllocator());	\
		}	\
	}

#define _M_push_func(DataType)																\
	bool Json::push(const String& name, const DataType value)								\
	{																						\
		if (isExist(name)) {																\
			auto itr = _pDoc->FindMember(name.to_wstring());								\
			if (itr->value.IsArray()) {														\
				itr->value.PushBack(JsonValueW(value), _pDoc->GetAllocator());				\
				return true;																\
			}																				\
			else {																			\
				return false;																\
			}																				\
		}																					\
		else {																				\
			JsonValueW arr(rapidjson::kArrayType);											\
			arr.PushBack(JsonValueW(value), _pDoc->GetAllocator());							\
			_pDoc->AddMember(makeValue(name.to_wstring()), arr, _pDoc->GetAllocator());		\
			return true;																	\
		}																					\
	}

#define _M_get_set_func(DataType, JsonType)		\
	_M_get_func(DataType, JsonType)				\
	_M_set_func(DataType, JsonType)				\
	_M_push_func(DataType)
	

_M_get_set_func(bool, Bool)
_M_get_set_func(int32_t, Int)
_M_get_set_func(uint32_t, Uint)
_M_get_set_func(int64_t, Int64)
_M_get_set_func(uint64_t, Uint64)
_M_get_set_func(float_t, Float)
_M_get_set_func(double_t, Double)


_M_get_func(String, String)

void Json::set(const String& name, const String& value)
{
	if (isExist(name)) {
		auto itr = _pDoc->FindMember(name.to_wstring());
		itr->value.SetString(value.to_wstring(), _pDoc->GetAllocator());
	}
	else {
		_pDoc->AddMember(makeValue(name), makeValue(value), _pDoc->GetAllocator());
	}
}

bool Json::push(const String& name, const String& value)
{
	if (isExist(name)) {
			auto itr = _pDoc->FindMember(name.to_wstring());
			if (itr->value.IsArray()) {
				itr->value.PushBack(makeValue(value), _pDoc->GetAllocator());
				return true;
			}
			else {
				return false;
			}
		}
	else {
		JsonValueW arr(rapidjson::kArrayType);
		arr.PushBack(makeValue(value), _pDoc->GetAllocator());
		_pDoc->AddMember(makeValue(name.to_wstring()), arr, _pDoc->GetAllocator());
		return true;
	}
}

#if 0
bool Json::get(const String& name, std::shared_ptr<Json> pValue)
{
	_M_is_exist;

	auto itr = _pDoc->FindMember(name.to_wstring());
	if (itr->value.IsObject()) {
		std::shared_ptr<JsonDocW> pDoc = std::make_shared<JsonDocW>();
		pDoc->CopyFrom(itr->value, pDoc->GetAllocator());

		if (!pValue) {
			pValue = std::make_shared<Json>();
		}
		pValue->_pDoc = pDoc;

		return true;
	}
	else {
		return false;
	}
}
#endif

bool Json::makeArray(const String& name)
{
	if (isExist(name)) {
		auto itr = _pDoc->FindMember(name.to_wstring());
		if (itr->value.IsArray()) {
			return true;
		}
		else {
			return false;
		}
	}
	else {
		JsonValueW arr(rapidjson::kArrayType);
		_pDoc->AddMember(makeValue(name), arr, _pDoc->GetAllocator());
		return true;
	}
}


JsonValueW Json::makeValue(const String& str)
{
	JsonValueW val;
	val.SetString(str.to_wstring(), _pDoc->GetAllocator());

	return val;
}


Json& Json::operator=(const Json& json)
{
	_pDoc = json._pDoc;

	return *this;
}


const String Json::toString(const bool useEnter) const
{
	if (!_pDoc->IsObject()) {
		return L"";
	}

	const_cast<Json*>(this)->update();

	JsonStringBufferW buffer;

	if (useEnter) {
		PrettyWriterW writer(buffer);
		_pDoc->Accept(writer);
	}
	else {
		WriterW writer(buffer);
		_pDoc->Accept(writer);
	}

	return String(buffer.GetString());
}

_namespace_soda_end