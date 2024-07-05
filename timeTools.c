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
    struct tm* tm = NULL;
    char str[11];
    sprintf(str, "%4d/%2d/%2d", datetime.year, datetime.month, datetime.day);
    strptime(str, "%Y/%m/%d", tm);
    return tm->tm_wday;
}