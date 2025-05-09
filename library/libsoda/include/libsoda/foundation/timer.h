#ifndef _LIBSODA_FOUNDATION_TIMER_H_
#define _LIBSODA_FOUNDATION_TIMER_H_

#include "libsoda/foundation/object.h"

_namespace_soda_begin

class Timer;

typedef void (Object::* TimerAction)(Timer*);

class Timer : public Object
{
public :
	Timer(const uint32_t interval, const bool repeat, Object* pTarget, TimerAction action)
		: _interval(interval), _repeat(repeat), _pTarget(pTarget), _action(action)
	{
	}

	virtual ~Timer() {};

public :
	uint32_t interval() const;
	bool isRepeat() const;
	Object* target() const;
	TimerAction action() const;

public :
	void setInterval(const uint32_t interval);
	void setRepeat(const bool repeat);
	void setTarget(Object* pTarget);
	void setAction(TimerAction action);

private :
	uint32_t _interval;
	bool _repeat;
	Object* _pTarget;
	TimerAction _action;
};	// class Timer

inline uint32_t Timer::interval() const { return _interval; }

inline bool Timer::isRepeat() const { return _repeat; }

inline Object* Timer::target() const { return _pTarget; }

inline TimerAction Timer::action() const { return _action; }


inline void Timer::setInterval(const uint32_t interval) { _interval = interval; }

inline void Timer::setRepeat(const bool repeat) { _repeat = repeat; }

inline void Timer::setTarget(Object* pTarget) { _pTarget = pTarget; }

inline void Timer::setAction(TimerAction action) { _action = action; }

_namespace_soda_end

#endif	// _LIBSODA_FOUNDATION_TIMER_H_