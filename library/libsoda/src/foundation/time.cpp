#include "libsoda/foundation/time.h"
#include "libsoda/foundation/time_formatter.h"
//#include "foundation/time_span.h"
#include "libsoda/foundation/system_util.h"

#if defined(_WIN64)
	#include <windows.h>

	#pragma warning(disable: 4996)
#else
	#include <unistd.h>
	#include <sys/time.h>
#endif

#include <cassert>

namespace {
	const int32_t s_monthDays[13] = {
		0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365
	};


	bool isValidTime(const int32_t year, const int32_t month, const int32_t day,
					 const int32_t hour, const int32_t min, const int32_t sec)
	{
		return (soda::Time::s_minYear <= year && year <= soda::Time::s_maxYear &&
				1 <= month && month <= 12 &&
				1 <= day && day <= soda::Time::daysInMonth(year, month) &&
				0 <= hour && hour <= 23 &&
				0 <= min && min <= 59 &&
				0 <= sec && sec <= 59);
	}


	struct tm convertToTM(const soda::Time& time)
	{
		struct tm t;
		t.tm_year = time.year() - 1900;
		t.tm_mon = time.month() - 1;
		t.tm_mday = time.day();
		t.tm_hour = time.hour();
		t.tm_min = time.min();
		t.tm_sec = time.sec();
		t.tm_isdst = -1;
		return t;
	}


	inline time_t32 platform_mktime(struct tm* lt)
	{
#if defined(_WIN64)
		return ::_mktime32(lt);
#else
		return static_cast<time_t32>(::mktime(lt));
#endif
	}


#if defined(_WIN64)
	inline bool platform_localtime(time_t32 t, struct tm* lt)
	{
		struct tm* p = ::_localtime32(&t);
		return (p) ? (*lt = *p, true) : false;
	}
#else
	inline bool platform_localtime(time_t32 t, struct tm* lt)
	{
		time_t tt = static_cast<time_t>(t);
		return (0 != ::localtime_r(&tt, lt)); // thread-safe version
	}
#endif


#if defined(_WIN32)
	inline bool platform_gmtime(time_t32 t, struct tm* gt)
	{
		struct tm* p = ::_gmtime32(&t);
		return (p) ? (*gt = *p, true) : false;
	}
#else
	inline bool platform_gmtime(time_t32 t, struct tm* gt)
	{
		time_t tt = static_cast<time_t>(t);
		return (0 != ::gmtime_r(&tt, gt)); // thread-safe version
	}
#endif

	inline uint32_t numberOfLeapDays(uint32_t from, uint32_t to)
	{
		from--; to--;
		return (to / 4 - from / 4) - (to / 100 - from / 100) + (to / 400 - from / 400);
	}

	const int32_t s_numberOfLeapDays[] = { // calculated by numberOfLeapDays()
		 0,  0,  0,  1,  1,  1,  1,  2,  2,  2,  2,  3,  3,  3,  3,  4,
		 4,  4,  4,  5,  5,  5,  5,  6,  6,  6,  6,  7,  7,  7,  7,  8,
		 8,  8,  8,  9,  9,  9,  9, 10, 10, 10, 10, 11, 11, 11, 11, 12,
		12, 12, 12, 13, 13, 13, 13, 14, 14, 14, 14, 15, 15, 15, 15, 16,
	};

	void checkLimit(int32_t& val, const int32_t min, const int32_t max)
	{
		if (val < min) {
			val = min;
		}

		if (val >= max) {
			val = max - 1;
		}
	}
}	// unnamed namespace


_namespace_soda_begin


Time::RtcFunc Time::_setRtc = 0;


bool isSameDate(const Time& t1, const Time& t2)
{
	return (t1.year() == t2.year() && t1.month() == t2.month() && t1.day() == t2.day());
}


bool isSameHour(const Time& t1, const Time& t2)
{
	return (isSameDate(t1, t2) && t1.hour() == t2.hour());
}


bool isSameMinute(const Time& t1, const Time& t2)
{
	return (isSameHour(t1, t2) && t1.min() == t2.min());
}


Time::Time(const int32_t year, const int32_t mon, const int32_t day,
		   const int32_t hour, const int32_t min, const int32_t sec)
{
	set(year, mon, day, hour, min, sec);
}


Time::Time()
{
	makeInvalid();
}


Time::Time(const Time& t)
	: Time()
{
	set(t);
}


Time::Time(const time_t32 t)
	: Time()
{
	from_time_t(t);
}


Time::Time(const struct tm& lt)
	: Time()
{
	from_struct_tm(lt);
}


Time::~Time()
{

}


bool Time::isLeapYear(const int32_t year)
{
	return (((year & 3) == 0 && (year % 100) != 0) || (year % 400) == 0);
}


int32_t Time::daysInMonth(const int32_t year, const int32_t month)
{
	assert(1 <= month && month <= 12);

	int32_t days = s_monthDays[month] - s_monthDays[month - 1];
	return (isLeapYear(year) && month == 2) ? days + 1 : days;
}


void Time::setCurrentTime(const Time& t)
{
	assert(t.isValid());

	assert(false, "setCurrentTime() not supported");
#if defined(_WIN64)
#else
#endif
}


bool Time::isValid() const
{
	return isValidTime(year(), month(), day(), hour(), min(), sec());
}


void Time::makeInvalid()
{
	_field.year = 0;
	_field.month = 13;
	_field.day = 0;
	_field.hour = 25;
	_field.minute = 61;
	_field.second = 61;
}


void Time::set(const int32_t year, const int32_t month, const int32_t day,
			   const int32_t hour, const int32_t minute, const int32_t second)
{
	if (isValidTime(year, month, day, hour, minute, second)) {
		_field.year = year - s_minYear;
		_field.month = month;
		_field.day = day;
		_field.hour = hour;
		_field.minute = minute;
		_field.second = second;
	}
	else {
		makeInvalid();
	}
}


time_t32 Time::to_time_t() const
{
	struct tm lt = convertToTM(*this);
	return ::platform_mktime(&lt);
}


struct tm Time::to_struct_tm() const
{
	struct tm lt = convertToTM(*this);
	::platform_mktime(&lt);
	return lt;
}


void Time::from_time_t(time_t32 t)
{
	struct tm lt;
	if (::platform_localtime(t, &lt)) {
		from_struct_tm(lt);
	}
	else {
		makeInvalid();
	}
}


void Time::from_struct_tm(const struct tm& t)
{
	set(t.tm_year + 1900, t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec);
}


time_t32 Time::utcTime() const
{
	struct tm gt = convertToTM(*this);

	int32_t year = gt.tm_year + 1900;
	int32_t mon = gt.tm_mon;

#if defined(_DEBUG)
	assert(year >= s_minYear && year < s_maxYear);
	assert(0 <= mon && mon < 12);
#else
	checkLimit(year, s_minYear, s_maxYear);
	checkLimit(mon, 0, 12);
#endif

	int32_t yearCount = year - s_minYear;
	uint32_t days = (yearCount * 365) + s_numberOfLeapDays[yearCount];
	days += s_monthDays[mon];
	if (mon > 1 && isLeapYear(year)) {
		days++;
	}
	days += (gt.tm_mday - 1);

	time_t32 hours = (time_t32)((days * 24) + gt.tm_hour);

	return (((hours * 3600) + gt.tm_min * 60) + gt.tm_sec);
}


void Time::setUtcTime(time_t32 t)
{
	struct tm gt;
	if (::platform_gmtime(t, &gt)) {
		from_struct_tm(gt);
	}
	else {
		makeInvalid();
	}
}


const String Time::toString() const
{
	if (!isValid()) {
		String s;
		s.format(L"%04d-%02d-%02d %02d:%02d:%02d",
			year(), month(), day(), hour(), min(), sec());
		return s;
	}

	String s;
	s.format(L"%@-%@-%@ %@:%@:%@", &TimeFormatter::FORMAT_YEAR4,
								   &TimeFormatter::FORMAT_MONTH,
								   &TimeFormatter::FORMAT_DAY,
								   &TimeFormatter::FORMAT_HOUR24,
								   &TimeFormatter::FORMAT_MINUTE,
								   &TimeFormatter::FORMAT_SECOND);
	return TimeFormatter(s).formatTime(*this);
}

_namespace_soda_end