#include "system/db/types/club/joined_member.h"

#include <Poco/DateTimeFormatter.h>
#include <Poco/DateTimeFormat.h>

_namespace_server_begin

JoinedMember::JoinedMember()
	: _memUid(0)
	, _userUid(0)
	, _name()
	, _eGrade(Grade::Max)
	, _eGender(Gender::Max)
	, _eLv(Level::Normal)
{

}


JoinedMember::JoinedMember(const JoinedMember& mem)
	: JoinedMember()
{
	*this = mem;
}


JoinedMember::~JoinedMember()
{

}


void JoinedMember::from(const TblRow& row)
{
	_memUid = row.get<0>();
	_userUid = row.get<1>();
	setGrade(row.get<2>());
	setLevel(row.get<3>());
	_name.fromUtf8(row.get<4>());
	_phoneNum.from_string(row.get<5>());
	setGender(row.get<6>());

	String date(Poco::DateTimeFormatter().format(row.get<7>(), Poco::DateTimeFormat::SORTABLE_FORMAT));
	_birthDay = date.substring(0, 10);
}


std::shared_ptr<soda::Json> JoinedMember::toJson() const
{
	std::shared_ptr<soda::Json> pDoc = std::make_shared<soda::Json>();

	pDoc->set(L"memberUid", _memUid);
	pDoc->set(L"userUid", _userUid);
	pDoc->set(L"name", _name);
	pDoc->set(L"grade", gradeNum());
	pDoc->set(L"gender", genderNum());
	pDoc->set(L"birthDay", _birthDay);
	pDoc->set(L"level", levelNum());
	pDoc->set(L"phoneNum", _phoneNum);

	return pDoc;
}


const String JoinedMember::toJsonStr() const
{
	return toJson()->toString();
}



JoinedMember& JoinedMember::operator=(const JoinedMember& mem)
{
	_memUid = mem._memUid;
	_userUid = mem._userUid;
	_name = mem._name;
	_eGrade = mem._eGrade;
	_eGender = mem._eGender;
	_eLv = mem._eLv;
	_phoneNum = mem._phoneNum;
	_birthDay = mem._birthDay;

	return *this;
}



JoinedMemberList::JoinedMemberList()
	: _joinedMemList{}
{

}


JoinedMemberList::JoinedMemberList(const JoinedMemberList& list)
	: JoinedMemberList()
{
	*this = list;
}


JoinedMemberList::~JoinedMemberList()
{
	_joinedMemList.clear();
}


void JoinedMemberList::from(const Tbl& tbl)
{
	_joinedMemList.clear();

	for (auto itr = tbl.begin(); itr != tbl.end(); ++itr) {
		JoinedMember mem;
		mem.from(*itr);

		_joinedMemList.push_back(mem);
	}
}


std::shared_ptr<soda::Json> JoinedMemberList::toJson() const
{
	std::shared_ptr<soda::Json> pDoc = std::make_shared<soda::Json>();

	for (auto itr = _joinedMemList.begin(); itr != _joinedMemList.end(); ++itr) {
		std::shared_ptr<soda::Json> pObj = itr->toJson();

		pDoc->push(L"joinedMemList", pObj.get());
	}

	return pDoc;
}


const String JoinedMemberList::toJsonStr() const
{
	soda::Json doc;

	for (auto itr = _joinedMemList.begin(); itr != _joinedMemList.end(); ++itr) {
		std::shared_ptr<soda::Json> pObj = itr->toJson();
		doc.push(L"joinedMemList", pObj.get());
	}

	return doc.toString();
}


JoinedMemberList& JoinedMemberList::operator=(const JoinedMemberList& list)
{
	_joinedMemList = list._joinedMemList;

	return *this;
}


_namespace_server_end