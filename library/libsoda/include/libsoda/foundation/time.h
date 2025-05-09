#ifndef _LIBSODA_FOUNDATION_TIME_H_
#define _LIBSODA_FOUNDATION_TIME_H_

#include "libsoda/foundation/object.h"

#include <ctime>

_namespace_soda_begin

class Time : public Object
{
public :
	static bool isLeapYear(const int32_t year);
	static int32_t daysInMonth(const int32_t year, const int32_t mon);

public :
	static Time currentTime();
	static void setCurrentTime(const Time& t);

public :
	static const int32_t s_minYear = 1970;
	static const int32_t s_maxYear = 2033;

	enum class DayOfWeek {
		Sun = 0,
		Mon,
		Tue,
		Wed,
		Thu,
		Fri,
		Sat,
	};

private :
	union {
		struct {
#if defined(__linux__)
			uint32_t year : 6;    // 0 ~ 63
			uint32_t month : 4;    // 1 ~ 12
			uint32_t day : 5;    // 1 ~ 31
			uint32_t hour : 5;    // 0 ~ 23
			uint32_t minute : 6;    // 0 ~ 59
			uint32_t second : 6;    // 0 ~ 59
#else
			uint32_t second : 6;    // 0 ~ 59
			uint32_t minute : 6;    // 0 ~ 59
			uint32_t hour : 5;    // 0 ~ 23
			uint32_t day : 5;    // 1 ~ 31
			uint32_t month : 4;    // 1 ~ 12
			uint32_t year : 6;    // 0 ~ 63
#endif
		} _field;
		uint32_t _value;
	};

public :
	Time();

	Time(const int32_t year, const int32_t mon, const int32_t day,
		 const int32_t hour = 0, const int32_t min = 0, const int32_t sec = 0);
	
	Time(const Time& t);

	Time(Time&& t) noexcept = default;

	explicit Time(const time_t32 t);
	
	explicit Time(const struct tm& lt);

	virtual ~Time();

public :
	bool isValid() const;
	int32_t year() const;
	int32_t month() const;
	int32_t day() const;
	int32_t hour() const;
	int32_t min() const;
	int32_t sec() const;

	DayOfWeek dayOfWeek() const;

public :
	void makeInvalid();
	void set(const int32_t year, const int32_t mon, const int32_t day,
			 const int32_t hour = 0, const int32_t min = 0, const int32_t sec = 0);
	void set(const Time& t);

public :
	Time& operator=(const Time& t);
	Time& operator=(Time&& t) noexcept = default;

public:
	bool operator==(const Time& t) const { return _value == t._value; }
	bool operator!=(const Time& t) const { return _value != t._value; }
	bool operator<=(const Time& t) const { return _value <= t._value; }
	bool operator< (const Time& t) const { return _value < t._value; }
	bool operator>=(const Time& t) const { return _value >= t._value; }
	bool operator> (const Time& t) const { return _value > t._value; }

public :
	time_t32 to_time_t() const;
	struct tm to_struct_tm() const;

	void from_time_t(const time_t32 t);
	void from_struct_tm(const struct tm& t);

	time_t32 utcTime() const;
	void setUtcTime(const time_t32 t);

public :
	uint32_t toUint32() const;
	void fromUint32(const uint32_t t);

public :
	const String toString() const;

public :
	typedef void (*RtcFunc)(const Time& time);
	static RtcFunc _setRtc;

	static void setRtcFunc(RtcFunc func) { _setRtc = func; }
};	// class Time

#if defined(_WIN64)
inline Time Time::currentTime() { return Time(_time32(0)); }
#else
inline Time Time::currentTime() { return Time(time(0)); }
#endif

inline int32_t Time::year() const { return s_minYear + _field.year; }

inline int32_t Time::month() const { return _field.month; }

inline int32_t Time::day() const { return _field.day; }

inline int32_t Time::hour() const { return _field.hour; }

inline int32_t Time::min() const { return _field.minute; }

inline int32_t Time::sec() const { return _field.second; }


inline void Time::set(const Time& t) { _value = t._value; }

inline Time& Time::operator=(const Time& t) { set(t); return *this; }

inline Time::DayOfWeek Time::dayOfWeek() const { return static_cast<DayOfWeek>(to_struct_tm().tm_wday); }

inline uint32_t Time::toUint32() const { return _value; }

inline void Time::fromUint32(const uint32_t i) { _value = i; }

_namespace_soda_end

#endif	// _LIBSODA_FOUNDATION_TIME_H_