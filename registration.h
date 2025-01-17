#ifndef REGISTRATION
#define REGISTRATION

#include "timeTools.h"
#define CLINIC_TIME_TITLE_LENGTH 40
#define REGISTRATION_TITLE_LENGTH 40

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
ClinicTimeRecord* getClinicTimeRecord(unsigned clinicTimeId, Datetime datetime);
int chooseClinicTime(Datetime datetime);
void printRegistration(unsigned registrationId, short showTitle);
int editRegistrationRecord(unsigned registrationId);
int checkRegistrationRecord(long long patientId);
int appendRegistration(long long patientId, unsigned clinicTimeId, Datetime datetime);
int assignRegistration(unsigned patientId);

#endif