#ifndef RECORD
#define RECORD

#include "timeTools.h"
#define RECORD_CONTENT_LENGTH 1000
#define RECORD_FILENAME_LENGTH 100
#define RECORD_TITLE_LENGTH 40

typedef struct record {
    unsigned long recordId;
    Datetime datetime;
    long long patientId;
    long long doctorId;
    char content[RECORD_CONTENT_LENGTH];
    // TODO: ¿ªÒ©
    // TODO: ×¡Ôº
} Record;

int loadRecordData();
int saveRecordData();
int appendRecord(long long doctorId);
int printRecord(unsigned long recordId);
int checkHistoryRecord(long long patientId);

#endif