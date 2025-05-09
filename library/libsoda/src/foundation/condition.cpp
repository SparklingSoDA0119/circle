#include "libsoda/foundation/condition.h"

#if defined(_WIN64)
	#include <Windows.h>
#else
	#include <linux/unistd.h>
	#include <sys/time.h>
	#include <error.h>
#endif

_namespace_soda_begin

Condition::Condition()
	: _signaled(false)
{
#if defined(_WIN64)
	_handle = ::CreateEvent(0, FALSE, FALSE, 0);
#else
	::pthread_cond_init(&_handle, 0);
#endif
}


Condition::~Condition()
{
#if defined(_WIN64)
	::CloseHandle(_handle);
#else
	::pthread_cond_destroy(&_handle);
#endif
}


bool Condition::wait(const uint32_t timeout)
{
    bool signaled = false;

    _lock.lock();
    if (isSignaled()) {
        _lock.unlock();
        signaled = true;
    }
    else {
#if defined(_WIN64)
        _lock.unlock();
        MSG msg;
        DWORD ret;
        DWORD waiting = (0 == timeout) ? INFINITE : timeout;
        do {
            if (::PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
                ::TranslateMessage(&msg);
                ::DispatchMessage(&msg);
            }
            ret = ::MsgWaitForMultipleObjects(1, &_handle,
                FALSE, waiting, QS_ALLINPUT);
        } while (ret != WAIT_OBJECT_0 && ret != WAIT_TIMEOUT);
        signaled = (ret == WAIT_OBJECT_0);
#else   // pthread
        int32_t ret = 0;
        if (0 < timeout) {
            struct timespec ts;
            struct timeval tv;
            ::gettimeofday(&tv, 0);

            uint64_t nsec = static_cast<uint64_t>(timeout) * 1000000 + (tv.tv_usec * 1000);
            ts.tv_sec = tv.tv_sec + nsec / 1000000000;
            ts.tv_nsec = nsec % 1000000000;
            ret = ::pthread_cond_timedwait(&_handle, &_lock.pthread_mutex(), &ts);
        }
        else {                      // inifite-wait
            ret = ::pthread_cond_wait(&_handle, &_lock.pthread_mutex());
        }
        _lock.unlock();
        signaled = (ret == 0);
#endif
    }

    reset();  // auto reset

    return signaled;
}


void Condition::signal()
{
    std::lock_guard<std::mutex> al(_lock);

#if defined(_WIN32)
    ::SetEvent(_handle);
    _signaled = true;
#else   // pthread
    if (!_signaled) {
        _signaled = true;
        ::pthread_cond_signal(&_handle);
    }
#endif
}


void Condition::reset()
{
    std::lock_guard<std::mutex> al(_lock);

    _signaled = false;
#if defined(_WIN32)
    ::ResetEvent(_handle);
#endif
}


_namespace_soda_end