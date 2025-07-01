#ifndef _CIRCLE_SERVER_SYSTEM_DB_TYPES_CLUB_JOINED_MEMBER_H_
#define _CIRCLE_SERVER_SYSTEM_DB_TYPES_CLUB_JOINED_MEMBER_H_

#include "define.h"

#include <Poco/Tuple.h>
#include <libsoda/document/json/json.h>

_namespace_server_begin

enum class Grade
{
	Baby = 0,
	Junior = 1,
	D,
	C,
	B,
	A,
	S,
	SS,
	SSS,
	Max,
};

enum class Gender
{
	Man,
	Woman,
	Max,
};

class JoinedMember
{
public :
	typedef Poco::Tuple<int32_t, int32_t, std::string, int32_t, int32_t> TblRow;

public :
	JoinedMember();
	JoinedMember(const JoinedMember& mem);

	virtual ~JoinedMember();

public :
	int32_t memUid() const;
	int32_t userUid() const;
	const String& name() const;
	Grade grade() const;
	int32_t gradeNum() const;
	Gender gender() const;
	int32_t genderNum() const;

public :
	void setMemUid(const int32_t uid);
	void setUserUid(const int32_t uid);
	void setName(const String& name);
	void setGrade(const int32_t num);
	void setGrade(const Grade eGrade);
	void setGender(const int32_t num);
	void setGender(const Gender eGender);

public :
	void from(const TblRow& row);
	std::shared_ptr<soda::Json> toJson() const;
	const String toJsonStr() const;

public :
	JoinedMember& operator=(const JoinedMember& mem);

private :
	int32_t _memUid;
	int32_t _userUid;
	String _name;
	Grade _eGrade;
	Gender _eGender;
};	// class JoinedMember

inline int32_t JoinedMember::memUid() const { return _memUid; }

inline int32_t JoinedMember::userUid() const { return _userUid; }

inline const String& JoinedMember::name() const { return _name; }

inline Grade JoinedMember::grade() const { return _eGrade; }

inline int32_t JoinedMember::gradeNum() const { return static_cast<int32_t>(_eGrade); }

inline Gender JoinedMember::gender() const { return _eGender; }

inline int32_t JoinedMember::genderNum() const { return static_cast<int32_t>(_eGender); }


inline void JoinedMember::setMemUid(const int32_t uid) { _memUid = uid; }

inline void JoinedMember::setUserUid(const int32_t uid) { _userUid = uid; }

inline void JoinedMember::setName(const String& name) { _name = name; }

inline void JoinedMember::setGrade(const Grade eGrade) { _eGrade = eGrade; }

inline void JoinedMember::setGrade(const int32_t num) { if (num >= static_cast<int32_t>(Grade::Max)) return; _eGrade = static_cast<Grade>(num); }

inline void JoinedMember::setGender(const Gender eGender) { _eGender = eGender; }

inline void JoinedMember::setGender(const int32_t num) { if (num >= static_cast<int32_t>(Gender::Max)) return; _eGender = static_cast<Gender>(num); }


class JoinedMemberList
{
public :
	typedef std::vector<JoinedMember::TblRow> Tbl;

public :
	JoinedMemberList();
	JoinedMemberList(const JoinedMemberList& list);

	virtual ~JoinedMemberList();

public :
	void from(const Tbl& tbl);
	std::shared_ptr<soda::Json> toJson() const;
	const String toJsonStr() const;
	

public :
	JoinedMemberList& operator=(const JoinedMemberList& list);

private :
	std::vector<JoinedMember> _joinedMemList;
};

_namespace_server_end


#endif	// _CIRCLE_SERVER_SYSTEM_DB_TYPES_CLUB_JOINED_MEMBER_H_