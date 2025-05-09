#ifndef _LIBSODA_FOUNDATION_SINGLETON_H_
#define _LIBSODA_FOUNDATION_SINGLETON_H_

#include "libsoda/foundation/object.h"

#include <mutex>
#include <cassert>

_namespace_soda_begin

template <class T>
class Singleton
{
public :
	Singleton() {
		assert(_allocated);
	}
	static T* get();

private :
	static std::unique_ptr<T> _pInstance;
	static std::mutex _lock;
	static bool _allocated;
};	// class Singleton


template <class T>
T* Singleton<T>::get()
{
	if (!_pInstance.get()) {
		std::lock_guard<std::mutex> al(_lock);
		if (!_pInstance.get()) {
			_allocated = true;
			_pInstance.reset(new T);
			_allocated = false;
		}
	}

	return _pInstance.get();
}


template <class T>
std::unique_ptr<T> Singleton<T>::_pInstance;

template <class T>
std::mutex Singleton<T>::_lock;

template <class T>
bool Singleton<T>::_allocated;

_namespace_soda_end

#endif	// _LIBSODA_FOUNDATION_SINGLETON_H_