#include "core/os/date_time.hpp"
#include "core/format.hpp"
#include <ctime>

DateTime DateTime::Now() {
    time_t     now = time(nullptr);
    tm  t = *localtime(&now);

    return {
        (u8)t.tm_sec,
        (u8)t.tm_min,
        (u8)t.tm_hour,
        (u8)t.tm_mday,
        (u8)(t.tm_mon + 1),
        (u16)(t.tm_year + 1900),
        (u8)(t.tm_wday + 1),
        (u16)(t.tm_yday + 1),
        (s8)(t.tm_isdst)
    };
}

String FmtDate(const DateTime& date_time) {
    return Format("%-%-%", (int)date_time.Year, (int)date_time.Month, (int)date_time.Day);
}

String FmtDateTime(const DateTime& date_time) {
    return Format("%-%-%:%:%:%", (int)date_time.Year, (int)date_time.Month, (int)date_time.Day, (int)date_time.Hours, (int)date_time.Minutes, (int)date_time.Seconds);
}
