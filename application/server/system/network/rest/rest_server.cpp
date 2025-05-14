#include "system/network/rest/rest_server.h"

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
	
}

_namespace_server_end