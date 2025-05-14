#include "libsoda/foundation/thread.h"
#include "libsoda/foundation/system_util.h"

#include <stdexcept>
#include <exception>
#include <typeinfo>
#include <new>
#include <cstdlib>
#include <chrono>
#include <cassert>

#if defined(_WIN64)
	#include <windows.h>
	#include <process.h>
#else
	#include <linux/unistd.h>
	#include <sys/prctl.h>
	#include <syscall.h>

	#if !defined(__ETI__)
		namespace {
			inline pid_t gettid() { return (pid_t)syscall(__NR_gettid); }
		}
	#else
		#define gettid getpid
	#endif
#endif

_namespace_soda_begin

uint32_t Thread::s_defaultAffinityMaskValue = 0;
const uint32_t& Thread::s_defaultAffinityMask = Thread::s_defaultAffinityMaskValue;


Thread::Thread(const String& name, ThreadPool* pPool)
	: _id(INVALID_ID)
	, _handle(0)
	, _name(name)
	, _ePriority(Priority::Normal)
	, _ePolicy(Policy::Normal)
	, _affinityMask(0)
	, _pPool(pPool ? pPool : &ThreadPool::defaultPool())
	, _eThreadStatus(ThreadStatus::Ready)
{

}


Thread::~Thread()
{

}


void Thread::setPriority(const Priority ePriority, const Policy ePolicy)
{
	assert(Priority::Lowest <= ePriority && ePriority <= Priority::Highest);

	_ePriority = ePriority;
	_ePolicy = ePolicy;

	if (!_handle) {
		return;
	}

	if (!isAlive()) {
		return;
	}


#if defined(_WIN64)
	int32_t pri;
	if (_ePolicy == Policy::RealTime) {
		pri = (Priority::Highest <= _ePriority) ? THREAD_PRIORITY_TIME_CRITICAL :
			  (Priority::Normal  <= _ePriority) ? THREAD_PRIORITY_HIGHEST :
			  (Priority::Lowest  <= _ePriority) ? THREAD_PRIORITY_ABOVE_NORMAL : THREAD_PRIORITY_NORMAL;
	}
	else {
		pri = (Priority::Highest <= _ePriority) ? THREAD_PRIORITY_ABOVE_NORMAL :
			  (Priority::Normal  <= _ePriority) ? THREAD_PRIORITY_NORMAL :
			  (Priority::Lowest  <= _ePriority) ? THREAD_PRIORITY_BELOW_NORMAL : THREAD_PRIORITY_LOWEST;
	}
	
	::SetThreadPriority(_handle, pri);
#else
	int32_t pol;
	struct sched_param stParam;
	::pthread_getschedparam(_handle, &pol, &param);
	if (_ePolicy == Policy::RealTime) {
		stParam.sched_priority = _priority;
		pol = SCHED_FIFO;
	}
	else if (_ePolicy == Policy::RealTimeRR) {
		stParam.sched_priority = static_cast<int32_t>(_ePriority);
		pol = SCHED_RR;
	}
	else {
		stParam.sched_priority = 0;
		pol = SCHED_OTHER;
	}

	::pthread_setschedparam(_handle, pol, &param);
#endif
}


bool Thread::setAffinityMask(const uint32_t mask)
{
	if (!_handle) {
		return false;
	}

	if (!isAlive()) {
		return false;
	}

#if defined(_WIN64)
	if (0 == ::SetThreadAffinityMask(_handle, static_cast<DWORD_PTR>(mask))) {
		return false;
	}

	_affinityMask = mask;
	return true;
#else
	cpu_set_t cpuset;
	CPU_ZERO(&cpuset);

	int32_t maxCores = ::sysconf(_SC_NPROCESSORS_ONLN);
	maxcores = 32 < maxCores ? 32 : maxCores;
	for (int32_t i = 0; i < maxCores; ++i) {
		if (mask & (0x1 << i)) {
			CPU_SET(i, &cpuset);
		}
	}

	if (::pthread_setaffinity_np(_handle, sizeof(cpu_set_t), &cpuset) < 0) {
		return false;
	}

	_affinityMask = mask;

	return true;
#endif
}


void Thread::readyThread()
{
	setThreadStatus(ThreadStatus::Starting);
	starting();
}


void Thread::initThread()
{
#if defined(_WIN64)
	_id = static_cast<uint32_t>(::GetCurrentThreadId());
#else
	_id = static_cast<uint32_t>(gettid());
#endif

	_pPool->regist(this);

	if (_affinityMask == 0 && s_defaultAffinityMask > 0) {
		_affinityMask = s_defaultAffinityMask;
	}

	if (_affinityMask > 0) {
		setAffinityMask(_affinityMask);
	}

	setPriority(_ePriority, _ePolicy);
}


void Thread::runThread()
{
#if defined(__linux__)
	::prctl(PR_SET_NAME, _name.c_str(), 0, 0, 0);
#endif

	started();

	setThreadStatus(ThreadStatus::Working);
	threadFunc();

	setThreadStatus(ThreadStatus::Finishing);
	finished();
}


void Thread::cleanupThread()
{
	setThreadStatus(ThreadStatus::Ready);

	_pPool->unregist(this);

#if defined(_WIN64)
	if (_handle) {
		::CloseHandle(_handle);
	}
#endif

	_id = INVALID_ID;
	terminated();
}


void Thread::setThreadStatus(const ThreadStatus eStatus)
{
	std::lock_guard<std::mutex> al(_statusLock);
	_eThreadStatus = eStatus;
}


bool Thread::startingThread()
{
	if (!_M_performWithoutException(&Thread::readyThread)) {
		setThreadStatus(ThreadStatus::Ready);
		return false;
	}

	return true;
}


void Thread::_coreThreadFunc()
{
	if (_M_performWithoutException(&Thread::initThread)) {
		_M_performWithoutException(&Thread::runThread);
	}
	_M_performWithoutException(&Thread::cleanupThread);
}

#if defined(_WIN64)
unsigned int WINAPI Thread::coreThreadFunc(void* pParam)
#else
void* Thread::coreThreadFunc(void* pParam)
#endif
{
	Thread* pThread = reinterpret_cast<Thread*>(pParam);
	pThread->_coreThreadFunc();

	return 0;
}


bool Thread::run(bool waitUntilStarted)
{
	assert(!isAlive());
	assert(!_handle);

	if (!startingThread()) {
		return false;
	}

	bool success;
#if defined(_WIN64)
	DWORD id;
	_handle = (HANDLE)(ULONG_PTR)
	::_beginthreadex(0, 0, coreThreadFunc, this, 0, reinterpret_cast<UINT*>(&id));
	int32_t errorCode = GetLastError();
	success = (_handle != 0);
#else
	int32_t errorCode = ::pthread_create(&_handle, 0, coreThreadFunc, this);
	success = (0 == errorCode);
#endif

	if (!success) {
		printf("Thread::run Failed %s(%d)\n", _name.to_string().c_str(), errorCode);
		setThreadStatus(ThreadStatus::Ready);
		return false;
	}

	if (waitUntilStarted) {
		while (_eThreadStatus == ThreadStatus::Starting) {
			Thread::yield();
		}
	}
	return true;
}


void Thread::quit()
{
	if (isAlive()) {
		quitRequest();
	}
}


void Thread::join()
{
	if (!_handle) {
		return;
	}

#if defined(_WIN64)
	while (isAlive()) {
		Thread::sleep(10);
	}
#else
	void* p = 0;
	::pthread_join(_handle, &p);
#endif

	_handle = 0;
}

#if defined(_WIN64)
void Thread::yield() { ::SwitchToThread(); }

void Thread::sleep(uint32_t msec) { ::Sleep(msec); }
#else
void Thread::yield() { ::sched_yield(); }

void Thread::sleep(uint32_t msec)
{
	struct timespec ts_sleep, ts_remaining;
	ts_sleep.tv_sec = msec / 1000;
	ts_sleep.tv_nsec = (msec % 1000) * 1000000;
	::nanosleep(&ts_sleep, &ts_remaining);
}
#endif


void Thread::checkTimers()
{
	std::lock_guard<std::mutex> al(_timerLock);

	uint64_t tick = util::tick::currentTick();
	std::list<timer_s>::iterator i = _timers.begin();
	while (i != _timers.end()) {
		Timer* timer = (*i).pTimer.get();
		if (timer->interval() <= util::tick::diffTick(i->lastCheckTick, tick)) {
			i->lastCheckTick = tick;
			(timer->target()->*timer->action())(timer);
			if (!timer->isRepeat()) {
				_timers.erase(i++);
			}
			else {
				++i;
			}
		}
		else {
			++i;
		}
	}
}


#undef addTimer
Timer* Thread::addTimer(uint32_t interval, bool repeat, Object* target, TimerAction action)
{
	std::lock_guard<std::mutex> al(_timerLock);

	if (0 < interval && target && action) {
		_timers.push_back(timer_s());
		timer_s& t = _timers.back();
		t.pTimer.reset(new Timer(interval, repeat, target, action));
		t.lastCheckTick = util::tick::currentTick();    // ÃÊ±âÈ­.
		return t.pTimer.get();
	}
	return 0;
}

void Thread::restartTimer(Timer* timer)
{
	std::lock_guard<std::mutex> al(_timerLock);

	std::list<timer_s>::iterator i;
	for (i = _timers.begin(); i != _timers.end(); ++i) {
		if (i->pTimer.get() == timer) {
			i->lastCheckTick = util::tick::currentTick();
			break;
		}
	}
}


void Thread::removeTimer(Timer* timer)
{
	std::lock_guard<std::mutex> al(_timerLock);

	std::list<timer_s>::iterator i;
	for (i = _timers.begin(); i != _timers.end(); ++i) {
		if (i->pTimer.get() == timer) {
			_timers.erase(i);
			break;
		}
	}
}


void Thread::removeAllTimers()
{
	std::lock_guard<std::mutex> al(_timerLock);

	_timers.clear();
}


const String Thread::toString() const
{
	String s;
	s.format(L"\"%@\"(%u)", &name(), id());
	return s;
}

_namespace_soda_end