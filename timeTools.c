#include <stdio.h>
#include <time.h>
#include "timeTools.h"

Datetime getDateTime() {
    Datetime datetime;
    time_t timestamp = time(NULL);
    struct tm* tm = localtime(&timestamp);
    datetime.year = tm->tm_year + 1900;
    datetime.month = tm->tm_mon + 1;
    datetime.day = tm->tm_mday;
    datetime.hour = tm->tm_hour;
    datetime.minute = tm->tm_min;
    return datetime;
}

Weekday getWeekday(Datetime datetime) {
    struct tm tm = { 0 }, * ptm = NULL;
    time_t t;
    tm.tm_year = datetime.year - 1900;
    tm.tm_mon = datetime.month - 1;
    tm.tm_mday = datetime.day;
    t = mktime(&tm);
    ptm = localtime(&t);
    return ptm->tm_wday;
}