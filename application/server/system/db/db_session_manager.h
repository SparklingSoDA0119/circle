#ifndef _CIRCLE_SERVER_SYSTEM_DB_DB_SESSION_MANAGER_H_
#define _CIRCLE_SERVER_SYSTEM_DB_DB_SESSION_MANAGER_H_

#include "system/db/db_session_param.h"

#include <Poco/Data/SessionPool.h>
#include <Poco/Data/RecordSet.h>

_namespace_server_begin

typedef Poco::Data::SessionPool			DbSessPool;
typedef Poco::Data::RecordSet			DbRecordSet;
typedef std::unique_ptr<DbRecordSet>	DbRecordSetPtr;
typedef Poco::Data::Session				DbSess;
typedef std::unique_ptr<DbSess>			DbSessPtr;
typedef std::map<String, String>		QueryMap;

class DbSessionManager
{
public :
	DbSessionManager();
	virtual ~DbSessionManager();

public :
	int32_t initialize(const DbSessionParam& param);
	int32_t tryConnect(const DbSessionParam& param);
	int32_t tryDisconnect();
	int32_t tryReconnect();

private:
	DbSessPtr getConnectedSession();
	void checkAvailable();

public :
	const String getDbVersion();

public :
	template <typename Type>
	int32_t select(const String& queryStr, Type* pData, DbSess* pSess = nullptr)
	{
		DbSessPtr pSession = nullptr;
		if (!pSess) {
			pSession = getConnectedSession();
			if (!pSession) {
				printf("Error: Can not get connected session.\n");
				return 1;
			}

			pSess = pSession.get();
		}

		if (!pSess->isConnected()) {
			printf("Error: DbSession is not connected.\n");
			return 1;
		}

		try {
			Poco::Data::Statement state(*pSess);
			state << queryStr.toUtf8Str(), Poco::Data::Keywords::into(*pData), Poco::Data::Keywords::now;
		}
		catch (const Poco::Exception& e) {
			std::cerr << "Error: " << e.displayText() << "(query: " << queryStr.to_string() << ")" << std::endl;
			return 2;
		}

		return 0;
	}

private :
	bool _isRegistered;
	bool _isConnected;
	bool _isAvailable;
	
private :
	DbSessionParam _param;
	std::unique_ptr<DbSessPool> _pSessPool;

};	// class DbSessionManager

_namespace_server_end

#endif	// _CIRCLE_SERVER_SYSTEM_DB_DB_SESSION_MANAGER_H_