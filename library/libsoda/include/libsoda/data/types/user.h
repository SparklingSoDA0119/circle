#ifndef _LIBSODA_DATA_TYPES_USER_H_
#define _LIBSODA_DATA_TYPES_USER_H_

#include "libsoda/foundation/string.h"
#include "libsoda/document/json/json.h"

_namespace_soda_data_begin

class User
{
public :
	User();
	User(const User& user);
	
	virtual ~User();

public :
	uint32_t uid() const;
	uint32_t grpUid() const;
	const String& name() const;
	const String& phone() const;
	const String& email() const;

public :
	void setUid(const uint32_t uid);
	void setGrpUid(const uint32_t uid);
	void setName(const String& name);
	void setPhone(const String& number);
	void setEmail(const String& email);

public :
	bool operator==(const User& user) const;
	bool operator!=(const User& user) const;

	User& operator=(const User& user);

public :
	int32_t fromJson(const String& str);
	JsonPtr toJson() const;

protected :
	uint32_t _uid;
	uint32_t _grpUid;
	String _name;
	String _phone;
	String _email;
};	// class User

inline uint32_t User::uid() const { return _uid; }

inline uint32_t User::grpUid() const { return _grpUid; }

inline const String& User::name() const { return _name; }

inline const String& User::phone() const { return _phone; }

inline const String& User::email() const { return _email; }


inline void User::setUid(const uint32_t uid) { _uid = uid; }

inline void User::setGrpUid(const uint32_t uid) { _grpUid = uid; }

inline void User::setName(const String& name) { _name = name; }

inline void User::setPhone(const String& num) { _phone = num; }

inline void User::setEmail(const String& email) { _email = email; }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


_namespace_soda_data_end

#endif	// _LIBSODA_DATA_TYPES_USER_H_