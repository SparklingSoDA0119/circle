#ifndef _CIRCLE_SERVER_SYSTEM_DB_DB_SESSION_PARAM_H_
#define _CIRCLE_SERVER_SYSTEM_DB_DB_SESSION_PARAM_H_

#include "define.h"

_namespace_server_begin

class DbSessionParam
{
public :
	DbSessionParam();
	DbSessionParam(const DbSessionParam& param);

	virtual ~DbSessionParam();

public :
	const String& host() const;
	uint32_t port() const;
	const String& dbName() const;
	const String& id() const;
	const String& pw() const;
	bool useAutoReConnect() const;

public :
	void setHost(const String& host);
	void setPort(const uint32_t port);
	void setDbName(const String& name);
	void setId(const String& id);
	void setPw(const String& pw);
	void setUseAutoReConnect(const bool use);

public :
	const String getConnectString() const;

public :
	bool operator==(const DbSessionParam& param) const;
	bool operator!=(const DbSessionParam& param) const;

	DbSessionParam& operator=(const DbSessionParam& param);

private :
	String _host;
	uint32_t _port;
	String _dbName;
	String _id;
	String _pw;
	bool _useAutoReConnect;
};	// class DbSessionParam

inline const String& DbSessionParam::host() const { return _host; }

inline uint32_t DbSessionParam::port() const { return _port; }

inline const String& DbSessionParam::dbName() const { return _dbName; }

inline const String& DbSessionParam::id() const { return _id; }

inline const String& DbSessionParam::pw() const { return _pw; }

inline bool DbSessionParam::useAutoReConnect() const { return _useAutoReConnect; }


inline void DbSessionParam::setHost(const String& host) { _host = host; }

inline void DbSessionParam::setPort(const uint32_t port) { _port = port; }

inline void DbSessionParam::setDbName(const String& name) { _dbName = name; }

inline void DbSessionParam::setId(const String& id) { _id = id; }

inline void DbSessionParam::setPw(const String& pw) { _pw = pw; }

inline void DbSessionParam::setUseAutoReConnect(const bool use) { _useAutoReConnect = use; }

_namespace_server_end

#endif	// _CIRCLE_SERVER_SYSTEM_DB_DB_SESSION_PARAM_H_