#ifndef _LIBSODA_FOUNDATION_LOOPER_H_
#define _LIBSODA_FOUNDATION_LOOPER_H_

#include "libsoda/foundation/thread.h"
#include "libsoda/foundation/condition.h"

_namespace_soda_begin

class Looper : public Thread
{
public :
	Looper(const String& name, ThreadPool* pPool = nullptr);

	virtual ~Looper();

public :
	uint32_t autoResumeInterval() const;

	void setAutoResumeInterval(const uint32_t interval);

protected :
	virtual void threadFunc();
	virtual void looperFunc() = 0;
	virtual void quitRequested();
	virtual void terminated();

protected :
	virtual void suspending() {}
	virtual void suspended() {}
	virtual void resumed() {}

public :
	bool isQuitRequested() const;
	void suspend();
	void resume();

protected :
	uint32_t _autoResumeInterval;

private :
	bool _quitRequested;
	Condition _condition;
};	// class Looper

inline uint32_t Looper::autoResumeInterval() const { return _autoResumeInterval; }

inline void Looper::setAutoResumeInterval(const uint32_t interval) { _autoResumeInterval = interval; }

inline void Looper::terminated() { _quitRequested = false; }

inline bool Looper::isQuitRequested() const { return _quitRequested; }

_namespace_soda_end

#endif	// _LIBSODA_FOUNDATION_LOOPER_H_