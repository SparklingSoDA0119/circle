#include "libsoda/foundation/object.h"
#include "libsoda/foundation/exception.h"
#include "libsoda/foundation/dump.h"

#include <stdexcept>
#include <exception>
#include <typeinfo>
#include <new>
#include <cstdlib>

#if defined(_WIN64)
	#include <Windows.h>
#endif

_namespace_soda_begin

void Object::exceptionCaught(Exception& e)
{
	//std::cout << __FUNCTION__ << ": " << e.toString() << std::endl;
	
	abort();
}


void Object::objectExceptionCaught(Object& e)
{
	//std::cout << __FUNCTION__ << ": " << e.toString() << std::endl;
	abort();
}


void Object::stdExceptionCaught(std::exception& e)
{
	//std::cout << __FUNCTION__ << ": " << e.what() << std::endl;
	abort();
}


void Object::unknownExceptionCaught()
{
	//std::cout << __FUNCTION__ << std::endl;
	abort();
}


#if defined(_MSC_VER)
namespace {
	void _win_performWithoutException(Object* pObj, void(Object::*func)())
	{ 
		__try {
			(pObj->*func)();
		}
		__except (SODA_DUMP(GetExceptionInformation(), L"Thread"), EXCEPTION_CONTINUE_SEARCH)
		{

		}
	}
}
#endif

#undef performWithoutException
bool Object::performWithoutException(void (Object::* func)())
{
	#define _M_catch_std_exception(name)	catch (std::name& e) { this->stdExceptionCaught(e); }
	
	try {
#if defined(_MSC_VER)
		_win_performWithoutException(this, func);
#else
		(this->*func());
#endif
		return true;
	}
	catch (Exception& e) { this->exceptionCaught(e); }
	catch (Object& obj) { this->objectExceptionCaught(obj); }
	_M_catch_std_exception(domain_error)
	_M_catch_std_exception(invalid_argument)
	_M_catch_std_exception(length_error)
	_M_catch_std_exception(out_of_range)
	_M_catch_std_exception(logic_error)
	// std::runtime_errors
	_M_catch_std_exception(range_error)
	_M_catch_std_exception(overflow_error)
	_M_catch_std_exception(runtime_error)
	// std::others
	_M_catch_std_exception(bad_alloc)
	_M_catch_std_exception(bad_cast)
	_M_catch_std_exception(bad_typeid)
	_M_catch_std_exception(bad_exception)
	_M_catch_std_exception(exception)
	catch (...) { this->unknownExceptionCaught(); }
	return false;
}

const String Object::toString() const
{
	return L"";
}

_namespace_soda_end