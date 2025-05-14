#include "libsoda/data/types/user.h"

_namespace_soda_data_begin

User::User()
	: _uid(0)
	, _grpUid(0)
	, _name()
	, _phone()
	, _email()
{

}


User::User(const User& user)
{
	*this = user;
}


User::~User()
{

}


bool User::operator==(const User& usr) const
{
	if (_uid == usr._uid       &&
		_grpUid == usr._grpUid &&
		_name == usr._name     &&
		_phone == usr._phone   &&
		_email == usr._email)
	{
		return true;
	}
	else {
		return false;
	}
}


bool User::operator!=(const User& usr) const
{
	return !operator==(usr);
}


User& User::operator=(const User& usr)
{
	_uid    = usr._uid;
	_grpUid = usr._grpUid;
	_name   = usr._name;
	_phone  = usr._phone;
	_email  = usr._email;

	return *this;
}


int32_t User::fromJson(const String& str)
{
	Json json;
	int32_t ret = json.parse(str);
	if (ret != 0) {
		return ret;
	}

	json.get(L"uid", &_uid);
	json.get(L"name", &_name);
	json.get(L"phone", &_phone);
	json.get(L"email", &_email);

	return 0;
}


JsonPtr User::toJson() const
{
	JsonPtr pJson = std::make_shared<Json>();

	pJson->set(L"uid", _uid);
	pJson->set(L"name", _name);
	pJson->set(L"phone", _phone);
	pJson->set(L"email", _email);

	return pJson;
}

_namespace_soda_data_end
