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