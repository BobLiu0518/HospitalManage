#ifndef TIME_TOOLS
#define TIME_TOOLS

typedef struct time {
    unsigned hour;
    unsigned minute;
} Time;

typedef struct datetime {
    unsigned year;
    unsigned month;
    unsigned day;
    unsigned hour;
    unsigned minute;
} Datetime;

Datetime getDateTime();

#endif