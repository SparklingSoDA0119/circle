#ifndef _LIBSODA_FOUNDATION_THREAD_POOL_H_
#define _LIBSODA_FOUNDATION_THREAD_POOL_H_

#include "libsoda/foundation/string.h"

#include <list>
#include <mutex>

_namespace_soda_begin

class Thread;

class ThreadPool : public Object
{
public :
	static ThreadPool& defaultPool();
	static uint32_t currentThreadId();
	static const Thread* currentThread(const ThreadPool* pGivenPool = nullptr);
	static const String& currentThreadName(const ThreadPool* pGivePool = nullptr);

public :
	ThreadPool(const String& name);
	virtual ~ThreadPool() {}

public :
	class Enumerator
	{
	public :
		typedef std::list<Thread*>::const_iterator	ThreadPoolIter;

	public :
		Enumerator(ThreadPoolIter begin, ThreadPoolIter end) : _iter(begin), _end(end) {}
		Enumerator(const Enumerator& e) : _iter(e._iter), _end(e._end) {}
		Enumerator() {}

	public :
		Thread* next() { return _iter != _end ? *_iter++ : nullptr; }

	private :
		ThreadPoolIter _iter;
		ThreadPoolIter _end;
	};	// Enumerator

public :
	void regist(Thread* pThread);
	void unregist(Thread* pThread);

public :
	const String& name() const;
	size_t size() const;
	Enumerator threadEnumerator() const;
	Thread* find(const uint32_t id) const;
	Thread* find(const String& name) const;

public :
	const String toString() const override;
	
private :
	String _name;
	std::list<Thread*> _pool;
	mutable std::mutex _lock;
};	// class ThreadPool

inline const String& ThreadPool::name() const { return _name; }

inline size_t ThreadPool::size() const { return _pool.size(); }

_namespace_soda_end

#endif	// _LIBSODA_FOUNDATION_THREAD_POOL_H_