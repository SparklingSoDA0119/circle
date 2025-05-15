#include "system/db/db_session_manager.h"

#include <Poco/Data/MySQL/Connector.h>
#include <libsoda/foundation/singlton.h>

_namespace_server_begin

DbSessionManager::DbSessionManager()
	: _isRegistered(false)
	, _isConnected(false)
	, _isAvailable(false)
	, _param()
	, _pSessPool(nullptr)
{

}


DbSessionManager::~DbSessionManager()
{
	printf("Destroy DbSessionManager..\n");
}


int32_t DbSessionManager::initialize(const DbSessionParam& param)
{
	Poco::Data::MySQL::Connector::registerConnector();
	_isRegistered = true;

	int32_t ret = tryConnect(param);
	if (ret != 0) {
		return ret;
	}

	while (true) {
		checkAvailable();

		if (_isAvailable) {
			break;
		}
		else {
			std::this_thread::sleep_for(std::chrono::milliseconds(300));
		}
	}

	return 0;
}


int32_t DbSessionManager::tryConnect(const DbSessionParam& param)
{
	if (!_isRegistered) {
		Poco::Data::MySQL::Connector::registerConnector();
		_isRegistered = true;
	}

	if (_pSessPool) {
		printf("Info: DbSessPool had been created already.\n");
		return 0;
	}

	try {
		_pSessPool = std::make_unique<DbSessPool>("MySQL", param.getConnectString().to_string());
	}
	catch (const Poco::Exception& e) {
		std::cerr << "Error: " << e.displayText() << std::endl;

		tryDisconnect();
		return 1;
	}

	bool active = _pSessPool->isActive();

	_param = param;
	_isConnected = true;
	
	return 0;
}


int32_t DbSessionManager::tryDisconnect()
{
	if (_pSessPool) {
		int32_t used = _pSessPool->used();
		if (used) {
			printf("Error: Can not disconnect DB session(used session: %d)\n", _pSessPool->referenceCount());
			return 1;
		}

		_pSessPool.reset();
		_pSessPool = nullptr;
	}

	Poco::Data::MySQL::Connector::unregisterConnector();
	_isRegistered = false;
	_isConnected = false;
	_isAvailable = false;

	return 0;
}


int32_t DbSessionManager::tryReconnect()
{
	int32_t ret = tryDisconnect();
	if (ret != 0) {
		return ret;
	}

	return tryConnect(_param);
}


DbSessPtr DbSessionManager::getConnectedSession()
{
	if (!_pSessPool) {
		printf("Available session pool is not exist.\n");
		return nullptr;
	}

	DbSessPtr pSess = nullptr;
	try {
		DbSess sess = _pSessPool->get();

		pSess = std::make_unique<DbSess>(_pSessPool->get());
	}
	catch (const Poco::Exception& e) {
		std::cerr << "Error: " << e.displayText() << std::endl;
		if (pSess) {
			pSess.reset();
			pSess = nullptr;
		}
	}

	return std::move(pSess);
}


void DbSessionManager::checkAvailable()
{
	if (!_isConnected) {
		return;
	}

	String dbVer = getDbVersion();
	if (dbVer.isEmpty()) {
		_isAvailable = false;
		return;
	}

	_isAvailable = true;
}


const String DbSessionManager::getDbVersion()
{
	String query = L"SELECT VERSION()";
	std::string dbVer;

	select(query, &dbVer);

	return dbVer;
}

_namespace_server_end