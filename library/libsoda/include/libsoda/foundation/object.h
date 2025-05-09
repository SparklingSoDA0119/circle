#ifndef _LIBSODA_FOUNDATION_OBJECT_H_
#define _LIBSODA_FOUNDATION_OBJECT_H_

#include "libsoda/foundation/type.h"

_namespace_soda_begin

class Exception;
class String;

class Object
{
public :
	Object() {}
	virtual ~Object() = 0;

public :
	virtual std::shared_ptr<Object> clone() const { return nullptr; }

protected :
	virtual void exceptionCaught(Exception& e);
	virtual void objectExceptionCaught(Object& obj);
	virtual void stdExceptionCaught(std::exception& e);
	virtual void unknownExceptionCaught();

public :
	virtual bool performWithoutException(void (Object::* func)());

#define _M_performWithoutException(func) performWithoutException(static_cast<void (Object::*)()>(func))

public :
	virtual size_t hash() const { return 0; }

public :
	virtual const String toString() const;
};	// class Object

inline Object::~Object() {}

_namespace_soda_end

#endif // _LIBSODA_FOUNDATION_OBJECT_H_