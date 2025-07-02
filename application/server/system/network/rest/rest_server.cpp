#include "system/network/rest/rest_server.h"

#include <libsoda/foundation/singlton.h>

#if defined(_WIN64)
	#if defined(_DEBUG)
		#pragma comment(lib, "cpprest_2_10d.lib")
	#else
		#pragma comment(lib, "cpprest_2_10.lib")
	#endif
#endif

_namespace_server_begin

RestServer::RestServer()
	: _isOpen(false)
	, _param()
	, _listener()
	, _pDbSessManager(nullptr)
	, _urls()
{

}


RestServer::~RestServer()
{

}


int32_t RestServer::initialize(const RestServerParam& param)
{
	if (_isOpen) {
		printf("Error: RestServer had been opened already.\n");
		return 1;
	}

	_param = param;

	int32_t ret = open();
	if (ret != 0) {
		printf("Error: Can not open RestServer.\n");
		return ret;
	}

	_pDbSessManager = soda::Singleton<DbSessionManager>::get();
	if (!_pDbSessManager) {
		printf("Error: Can not get DbSessionManager.\n");
		return 1;
	}

	return 0;
}


int32_t RestServer::open()
{
	web::http::uri_builder builder = _param.getUrlBuilder();
	
	_listener = web::http::experimental::listener::http_listener(builder.to_string());

	_listener.support(std::bind(&RestServer::handleAll, this, std::placeholders::_1));

	try {
		_listener.open().then([&]() {
			printf("Info: RestServer is opened..\n");
		}).wait();
	}
	catch (web::http::http_exception& e) {
		printf("Error: RestServer open error(http_exception: %s)", e.what());
		return 2;
	}
	catch (std::exception& e) {
		printf("Error: RestServer open error(std::exception: %s)", e.what());
		return 3;
	}
	catch (...) {
		printf("Error: RestServer open error(unknown exception)");
		return 4;
	}

	_isOpen = true;

	return 0;
}


void RestServer::handleAll(const web::http::http_request& req)
{
	RestRequest request(req);

	String uri = request.path();

	if (uri.find(L"circle/test/club") != String::npos) {
		Response resp = handleClub(&request);
		request.replyJson(web::http::status_codes::OK, &resp.msg());
		return;
	}
}


Response RestServer::handleClub(const RestRequest* pReq)
{
	String uri = pReq->path();
	String mode = pReq->findHeader(L"mode");
	String clubGuid = pReq->findHeader(L"guid");

	if (mode.isEmpty() || clubGuid.isEmpty()) {
		return Response(404, L"mode or guid is empty.");
	}
	else if (mode == L"information") {
		String result = _pDbSessManager->searchClubInformation(clubGuid);
		if (result.isEmpty()) {
			return Response(500, L"search error.");
		}
		else {
			return Response(200, result);
		}
	}

	return Response();
}

_namespace_server_end