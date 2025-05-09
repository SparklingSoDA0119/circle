#ifndef _LIBSODA_FOUNDATION_TIME_FORMATTER_H_
#define _LIBSODA_FOUNDATION_TIME_FORMATTER_H_

#include "libsoda/foundation/string.h"

_namespace_soda_begin

class Time;

class TimeFormatter : public Object
{
public :
	TimeFormatter(const String& fmt = L"");
	TimeFormatter(const TimeFormatter& fmter);

	virtual ~TimeFormatter();

public :
	bool isValid() const;
	const String& format() const;
	const String& amStr() const;
	const String& pmStr() const;

	void setFormat(const String& fmt);
	void setAmPmStr(const String& am, const String& pm);
    const String formatTime(const Time& time) const;

public :
	bool operator==(const TimeFormatter& fmter) const;
	bool operator!=(const TimeFormatter& fmter) const;

public :
	const String toString() const override;

public :
    static const String FORMAT_YEAR4;
    static const String FORMAT_YEAR2;
    static const String FORMAT_MONTH;
    static const String FORMAT_DAY;
    static const String FORMAT_HOUR24;
    static const String FORMAT_HOUR12;
    static const String FORMAT_MINUTE;
    static const String FORMAT_SECOND;
    static const String FORMAT_AMPM;

public :
    static String format_YYYY_MM_DD(wchar_t dateSeparator = '-');
    static String format_MM_DD_YYYY(wchar_t dateSeparator = '-');
    static String format_DD_MM_YYYY(wchar_t dateSeparator = '-');
    static String format_YY_MM_DD(wchar_t dateSeparator = '-');
    static String format_MM_DD_YY(wchar_t dateSeparator = '-');
    static String format_DD_MM_YY(wchar_t dateSeparator = '-');
    static String format_H24_MM();
    static String format_H24_MM_SS();
    static String format_H12_MM_AP();
    static String format_H12_MM_SS_AP();
    static String format_AP_H12_MM();
    static String format_AP_H12_MM_SS();

private :
	String _fmter;
	String _amStr;
	String _pmStr;
};	// class TimeFormatter

inline bool TimeFormatter::isValid() const { return !_fmter.isEmpty(); }

inline const String& TimeFormatter::format() const { return _fmter; }

inline const String& TimeFormatter::amStr() const { return _amStr; }

inline const String& TimeFormatter::pmStr() const { return _pmStr; }


inline void TimeFormatter::setFormat(const String& fmt) { _fmter = fmt; }

inline void TimeFormatter::setAmPmStr(const String& am, const String& pm) { _amStr = am; _pmStr = pm; }

_namespace_soda_end

#endif	// _LIBSODA_FOUNDATION_TIME_FORMATTER_H_