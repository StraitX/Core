#pragma once

#include "core/types.hpp"
#include "core/printer.hpp"
#include "core/string.hpp"

struct DateTime {
	u8 Seconds = 0;
	u8 Minutes = 0;
	u8 Hours = 0;
	u8 Day = 0;
	u8 Month = 0;
	u16 Year = 0;
	u8 DayOfWeek = 0;
	u16 DayOfYear = 0;
	s8 IsDst = 0;


	static DateTime Now();
};


String FmtDate(const DateTime &date_time);

