#include "libsoda/foundation/time_formatter.h"
#include "libsoda/foundation/time.h"

#include <algorithm>
#include <iostream>

_namespace_soda_begin

TimeFormatter::TimeFormatter(const String& fmt)
{
    setFormat(fmt);
}


TimeFormatter::TimeFormatter(const TimeFormatter& fmter)
{
	setFormat(fmter.format());
	setAmPmStr(fmter.amStr(), fmter.pmStr());
}


TimeFormatter::~TimeFormatter()
{

}


const String TimeFormatter::formatTime(const Time& time) const
{
    static const std::wstring types(L"@AaBbCcDdEeFGgHhIjklMmnOpRrSsTtUuVvWwXxYyZz");

    struct tm tm = time.to_struct_tm();
    const String& ampm = (time.hour() < 12) ? _amStr : _pmStr;

    const uint32_t TS_N = 256;
    wchar_t ts[TS_N];
    std::wstring tf, result;
    const std::wstring& format = _fmter.to_wstring();
    std::wstring::const_iterator si = format.begin();
    std::wstring::const_iterator t, i = si;

    while (true) {
        i = std::find(i, format.end(), L'%');
        if (i == format.end()) {
            result.append(si, format.end());
            break;
        }

        if (*(i + 1) == L'%') {
            result.append(si, i + 1);
            i += 2;
            si = i;
            continue;
        }

        t = std::find_first_of(i, format.end(), types.begin(), types.end());
        if (t == format.end()) {
            std::wcerr << __FUNCTION__ << L"(): Unknown type " << "\"%" << *(i + 1) << "\"" << std::endl;
            abort();
            break;
        }

        result.append(si, i);

        tf.assign(i, t + 1);
        if (*t == L'p' && !ampm.isEmpty()) {
            result.append(ampm.to_wstring());
        }
        else {
            ::wcsftime(ts, TS_N, tf.c_str(), &tm);
            result.append(ts);
        }

        i = t + 1;
        si = i;
    }

    return String(result.c_str());
}


bool TimeFormatter::operator==(const TimeFormatter& tf) const
{
    return _fmter == tf._fmter && _amStr == tf._amStr && _pmStr == tf._pmStr;
}


bool TimeFormatter::operator!=(const TimeFormatter& tf) const
{
    return !operator==(tf);
}


const String TimeFormatter::toString() const
{
    String s;
    s.format(L"format=\"%@\", am=\"%@\", pm=\"%@\"", &_fmter, &_amStr, &_pmStr);
    return s;
}


const String TimeFormatter::FORMAT_YEAR4(L"%Y");
const String TimeFormatter::FORMAT_YEAR2(L"%y");
const String TimeFormatter::FORMAT_MONTH(L"%m");
const String TimeFormatter::FORMAT_DAY(L"%d");
const String TimeFormatter::FORMAT_HOUR24(L"%H");
const String TimeFormatter::FORMAT_HOUR12(L"%I");
const String TimeFormatter::FORMAT_MINUTE(L"%M");
const String TimeFormatter::FORMAT_SECOND(L"%S");
const String TimeFormatter::FORMAT_AMPM(L"%p");


String TimeFormatter::format_YYYY_MM_DD(wchar_t dateSeparator)
{
    String s;
    s.format(L"%@%c%@%c%@", &FORMAT_YEAR4, dateSeparator, &FORMAT_MONTH, dateSeparator, &FORMAT_DAY);
    return s;
}


String TimeFormatter::format_MM_DD_YYYY(wchar_t dateSeparator)
{
    String s;
    s.format(L"%@%c%@%c%@", &FORMAT_MONTH, dateSeparator, &FORMAT_DAY, dateSeparator, &FORMAT_YEAR4);
    return s;
}


String TimeFormatter::format_DD_MM_YYYY(wchar_t dateSeparator)
{
    String s;
    s.format(L"%@%c%@%c%@", &FORMAT_DAY, dateSeparator, &FORMAT_MONTH, dateSeparator, &FORMAT_YEAR4);
    return s;
}


String TimeFormatter::format_YY_MM_DD(wchar_t dateSeparator)
{
    String s;
    s.format(L"%@%c%@%c%@", &FORMAT_YEAR2, dateSeparator, &FORMAT_MONTH, dateSeparator, &FORMAT_DAY);
    return s;
}


String TimeFormatter::format_MM_DD_YY(wchar_t dateSeparator)
{
    String s;
    s.format(L"%@%c%@%c%@", &FORMAT_MONTH, dateSeparator, &FORMAT_DAY, dateSeparator, &FORMAT_YEAR2);
    return s;
}


String TimeFormatter::format_DD_MM_YY(wchar_t dateSeparator)
{
    String s;
    s.format(L"%@%c%@%c%@", &FORMAT_DAY, dateSeparator, &FORMAT_MONTH, dateSeparator, &FORMAT_YEAR2);
    return s;
}


String TimeFormatter::format_H24_MM()
{
    String s;
    s.format(L"%@:%@", &FORMAT_HOUR24, &FORMAT_MINUTE);
    return s;
}


String TimeFormatter::format_H24_MM_SS()
{
    String s;
    s.format(L"%@:%@:%@", &FORMAT_HOUR24, &FORMAT_MINUTE, &FORMAT_SECOND);
    return s;
}


String TimeFormatter::format_H12_MM_AP()
{
    String s;
    s.format(L"%@:%@ %@", &FORMAT_HOUR12, &FORMAT_MINUTE, &FORMAT_AMPM);
    return s;
}


String TimeFormatter::format_H12_MM_SS_AP()
{
    String s;
    s.format(L"%@:%@:%@ %@", &FORMAT_HOUR12, &FORMAT_MINUTE, &FORMAT_SECOND, &FORMAT_AMPM);
    return s;
}


String TimeFormatter::format_AP_H12_MM()
{
    String s;
    s.format(L"%@ %@:%@", &FORMAT_AMPM, &FORMAT_HOUR12, &FORMAT_MINUTE);
    return s;
}


String TimeFormatter::format_AP_H12_MM_SS()
{
    String s;
    s.format(L"%@ %@:%@:%@", &FORMAT_AMPM, &FORMAT_HOUR12, &FORMAT_MINUTE, &FORMAT_SECOND);
    return s;
}


_namespace_soda_end