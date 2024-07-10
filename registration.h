#ifndef REGISTRATION
#define REGISTRATION

#include "timeTools.h"
#define CLINIC_TIME_TITLE_LENGTH 40

typedef enum {
    valid, invalid
} Status;
typedef struct {
    unsigned clinicTimeId;
    long long doctorId;
    Weekday weekday;
    Time startTime;
    Time endTime;
    unsigned maxAmount;
    Status status;
} ClinicTime;
typedef struct {
    unsigned clinicTimeId;
    Datetime datetime;
    unsigned remainAmount;
} ClinicTimeRecord;
typedef struct {
    unsigned registrationId;
    unsigned clinicTimeId;
    Datetime datetime;
    long long doctorId;
    long long patientId;
    Status status;
} RegistrationRecord;

int loadClinicTime();
int loadClinicTimeRecord();
int loadRegistrations();
int saveClinicTime();
int saveClinicTimeRecord();
int saveRegistrations();
unsigned addClinicTime(long long doctorId);
int editClinicTime(unsigned clinicTimeId, char* title);
int manageClinicTime(long long doctorId);
int setClinicTimeStatus(unsigned clinicTimeId, Status status);
ClinicTimeRecord* getClinicTimeRecord(unsigned clinicTimeId, Datetime datetime);
unsigned chooseClinicTime(Datetime datetime);
void printRegistration(unsigned registrationId, short showTitle);
int appendRegistration(long long patientId, unsigned clinicTimeId, Datetime datetime);
int assignRegistration(unsigned patientId);

#endif