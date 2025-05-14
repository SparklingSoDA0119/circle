#ifndef _LIBSODA_FOUNDATION_THREAD_H_
#define _LIBSODA_FOUNDATION_THREAD_H_

#include "libsoda/foundation/thread_pool.h"
#include "libsoda/foundation/timer.h"

#if defined(__linux__)
	#include <pthread.h>
	#include <unistd.h>
	#include <sys/types.h>
#endif

#include <unordered_map>
#include <list>

_namespace_soda_begin

class Thread : public Object
{
public :
#if defined(_WIN64)
	typedef void* Handle;
#else
	typedef pthread_t Handle;
#endif

public:
	static const uint32_t INVALID_ID = 0;

private :
	static uint32_t s_defaultAffinityMaskValue;

public :
	static const uint32_t& s_defaultAffinityMask;
	static void setDefaultThreadAffinityMask(const uint32_t mask) { s_defaultAffinityMaskValue = mask; }
	
public :
	enum class Priority {
		Lowest = 1,
		Normal = 50,
		Highest = 99,
	};

public:
	enum class Policy {
		Normal = 0,
		RealTime = 1,
#if defined(__linux__)
		RealTimeRR = 2,
#endif
	};

public:
	enum class ThreadStatus
	{
		Ready = 0,
		Starting = 1,
		Working = 2,
		Suspending = 3,
		Suspended = 4,
		Finishing = 5,
	};

protected :
	struct timer_s {
		std::shared_ptr<Timer> pTimer;
		uint64_t lastCheckTick;
		timer_s() : lastCheckTick(0) {}
	};

public :
	Thread(const String& name, ThreadPool* pPool = nullptr);

	virtual ~Thread();

public :
	uint32_t id() const;
	Handle& handle();
	const String& name() const;
	Priority priority() const;
	Policy policy() const;
	uint32_t affinityMask() const;
	ThreadPool* pool() const;
	ThreadStatus threadStatus() const;

public :
	void setName(const String& name);
	void setPriority(const Priority ePriority, const Policy ePolicy);
	bool setAffinityMask(const uint32_t mask);

public :
	bool isAlive() const;
	bool run(bool waitUntilStarted = false);
	void quit();
	void join();

public :
	static void yield();
	static void sleep(const uint32_t ms);

private :
	void readyThread();
	void initThread();
	void runThread();
	void cleanupThread();
	void _coreThreadFunc();

protected :
	void setThreadStatus(const ThreadStatus eStatus);
	bool startingThread();

#if defined(_WIN64)
	static unsigned int WINAPI coreThreadFunc(void* pParam);
#else
	static void* coreThreadFunc(void* pParam);
#endif

protected :
	virtual void starting() {}
	virtual void started() {}
	virtual void threadFunc() = 0;
	virtual void quitRequest() {}
	virtual void finished() {}
	virtual void terminated() {}

protected :
	void checkTimers();
	
public :
	Timer* addTimer(const uint32_t interval, const bool repeat, Object* pTarget, TimerAction action);
	void restartTimer(Timer* pTimer);
	void removeTimer(Timer* pTimer);
	void removeAllTimers();

public :
	const String toString() const override;

private :
	uint32_t _id;
	Handle _handle;
	String _name;
	Priority _ePriority;
	Policy _ePolicy;
	uint32_t _affinityMask;
	ThreadPool* _pPool;

private :
	ThreadStatus _eThreadStatus;
	std::mutex _statusLock;

private :
	std::list<timer_s> _timers;
	std::mutex _timerLock;
};	// class Thread

inline uint32_t Thread::id() const { return _id; }

inline Thread::Handle& Thread::handle() { return _handle; }

inline const String& Thread::name() const { return _name; }

inline Thread::Priority Thread::priority() const { return _ePriority; }

inline Thread::Policy Thread::policy() const { return _ePolicy; }

inline uint32_t Thread::affinityMask() const { return _affinityMask; }

inline ThreadPool* Thread::pool() const { return _pPool; }

inline Thread::ThreadStatus Thread::threadStatus() const { return _eThreadStatus; }


inline void Thread::setName(const String& name) { _name = name; }


inline bool Thread::isAlive() const { return _eThreadStatus == ThreadStatus::Starting || _id != INVALID_ID; }


_namespace_soda_end

#endif	// _LIBSODA_FOUNDATION_THREAD_H_