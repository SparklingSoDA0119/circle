#include "libsoda/foundation/looper.h"

_namespace_soda_begin

Looper::Looper(const String& name, ThreadPool* pPool)
	: Thread(name, pPool)
	, _autoResumeInterval(0)
	, _quitRequested(false)
{

}


Looper::~Looper()
{
    quit();
    join();
}


void Looper::threadFunc()
{
    while (!isQuitRequested()) {
        if (threadStatus() == ThreadStatus::Suspending) {
            setThreadStatus(ThreadStatus::Suspended);
            suspended();

            _condition.wait(_autoResumeInterval);
            _condition.reset();
            if (!isQuitRequested()) {
                setThreadStatus(ThreadStatus::Working);
                resumed();
            }
        }
        else {
            looperFunc();
        }
    }
}


void Looper::quitRequested()
{
    _quitRequested = true;
    resume();   // wake up
}


void Looper::suspend()
{
    while (threadStatus() == ThreadStatus::Starting) {
        Thread::yield();
    }

    if (threadStatus() == ThreadStatus::Working) {
        _condition.reset();
        setThreadStatus(ThreadStatus::Suspending);
        suspending();
    }
    else {
        // do nothing
    }
}


void Looper::resume()
{
    if (threadStatus() != ThreadStatus::Working) {
        _condition.signal();
    }
    else {
        // do nothing
    }
}

_namespace_soda_end