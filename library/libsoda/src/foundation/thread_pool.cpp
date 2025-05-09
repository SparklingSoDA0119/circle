#include "libsoda/foundation/thread_pool.h"
#include "libsoda/foundation/thread.h"

#if defined(_WIN64)
	#include <Windows.h>
#else
	#include <syscall.h>
	namespace {
		inline pid_t gettid() { return (pid_t)syscall(__NR_gettid); }
	}
#endif

_namespace_soda_begin

ThreadPool& ThreadPool::defaultPool()
{
	static ThreadPool s_defaultPool(L"Default Thread Pool");
	return s_defaultPool;
}


ThreadPool::ThreadPool(const String& name)
	: _name(name)
{

}


uint32_t ThreadPool::currentThreadId()
{
	uint32_t id = std::numeric_limits<uint32_t>::max();

#if defined(_WIN64)
	id = static_cast<uint32_t>(::GetCurrentThreadId());
#else
	id = static_Cast<uint32_t>(::gettid());
#endif

	return id;
}


const Thread* ThreadPool::currentThread(const ThreadPool* pGivenPool)
{
	uint32_t id = currentThreadId();

	Thread* pThread = nullptr;
	if (pGivenPool) {
		pThread = pGivenPool->find(id);
	}
	else {
		pThread = defaultPool().find(id);
	}

	return (const Thread*)pThread;
}


const String& ThreadPool::currentThreadName(const ThreadPool* pGivenPool)
{
	static String s_unknownThread(L"UnknownThread");

	const Thread* pThread = ThreadPool::currentThread(pGivenPool);
	if (pThread) {
		return pThread->name();
	}

	return s_unknownThread;
}


void ThreadPool::regist(Thread* pThread)
{
	std::lock_guard<std::mutex> al(_lock);
	_pool.push_back(pThread);
}


void ThreadPool::unregist(Thread* pThread)
{
	std::lock_guard<std::mutex> al(_lock);
	_pool.remove(pThread);
}


ThreadPool::Enumerator ThreadPool::threadEnumerator() const
{
	return Enumerator(_pool.begin(), _pool.end());
}


namespace {
	struct IdFinder {
		uint32_t _id;
		IdFinder(uint32_t id) : _id(id) {}
		bool operator()(Thread* pThread) { return (pThread->id() == _id); }
	};

	struct NameFinder {
		String _name;
		NameFinder(const String& name) : _name(name) {}
		bool operator()(Thread* pThread) { return (pThread->name() == _name); }
	};
}


Thread* ThreadPool::find(const uint32_t id) const
{
	std::lock_guard<std::mutex> al(_lock);
	std::list<Thread*>::const_iterator i;

	i = std::find_if(_pool.begin(), _pool.end(), IdFinder(id));
	return i != _pool.end() ? *i : nullptr;
}


Thread* ThreadPool::find(const String& name) const
{
	std::lock_guard<std::mutex> al(_lock);
	std::list<Thread*>::const_iterator i;

	i = std::find_if(_pool.begin(), _pool.end(), NameFinder(name));
	return i != _pool.end() ? *i : nullptr;
}


const String ThreadPool::toString() const
{
	String s;
	s.format(L"\"%@\"={ ", &name());
	std::list<Thread*>::const_iterator i = _pool.begin();
	while (i != _pool.end()) {
		s.appendf(L"[%@]", *i);
		if (++i != _pool.end()) {
			s.append(L", ");
		}
	}
	s.append(L" }");
	return s;
}

_namespace_soda_end