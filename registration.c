#include <stdio.h>
#include <stdlib.h>
#include "beautifulDisplay.h"
#include "dynamicArray.h"
#include "fileTools.h"
#include "timeTools.h"
#include "033.h"
#include "registration.h"

DynamicArray clinicTimes = { 0 };
DynamicArray registrations = { 0 };
DynamicArray clinicTimeRecords = { 0 };

int loadClinicTime() {
    if (clinicTimes.ptr) {
        freeDynamicArray(&clinicTimes);
    }
    initDynamicArray(&clinicTimes, 128, sizeof(ClinicTime));
    int c;
    FILE* fp = NULL;
    ClinicTime clinicTime;
    if (fileOpen(&fp, "storage\\clinicTime.csv", "r")) {
        return -1;
    }
    while (!feof(fp)) {
        c = fscanf(fp, "%u,%lld,%d,%2u%2u,%2u%2u,%u,%d\n", &clinicTime.clinicTimeId, &clinicTime.doctorId, &clinicTime.weekday, &clinicTime.startTime.hour,
            &clinicTime.startTime.minute, &clinicTime.endTime.hour, &clinicTime.endTime.minute, &clinicTime.maxAmount, &clinicTime.status);
        if (c == EOF) {
            break;
        }
        appendItem(&clinicTimes, &clinicTime);
    }
    fclose(fp);
    return 0;
}

int loadClinicTimeRecord() {
    if (clinicTimeRecords.ptr) {
        freeDynamicArray(&clinicTimeRecords);
    }
    initDynamicArray(&clinicTimeRecords, 128, sizeof(ClinicTimeRecord));
    int c;
    FILE* fp = NULL;
    ClinicTimeRecord record;
    if (fileOpen(&fp, "storage\\clinicTimeRecord.csv", "r")) {
        return -1;
    }
    while (!feof(fp)) {
        c = fscanf(fp, "%u,%4u%2u%2u,%u\n", &record.clinicTimeId, &record.datetime.year, &record.datetime.month,
            &record.datetime.day, &record.remainAmount);
        if (c == EOF) {
            break;
        }
        appendItem(&clinicTimeRecords, &record);
    }
    fclose(fp);
    return 0;
}

int loadRegistrations() {
    if (registrations.ptr) {
        freeDynamicArray(&registrations);
    }
    initDynamicArray(&registrations, 128, sizeof(RegistrationRecord));
    int c;
    char datetime[15];
    FILE* fp = NULL;
    RegistrationRecord registration;
    if (fileOpen(&fp, "storage\\registration.csv", "r")) {
        return -1;
    }
    while (!feof(fp)) {
        c = fscanf(fp, "%u,%u,%[^,],%lld,%lld,%d\n", &registration.registrationId, &registration.clinicTimeId,
            datetime, &registration.doctorId, &registration.patientId, &registration.status);
        if (c == EOF) {
            break;
        }
        sscanf(datetime, "%4u%2u%2u%2u%2u", &registration.datetime.year, &registration.datetime.month,
            &registration.datetime.day, &registration.datetime.hour, &registration.datetime.minute);
        appendItem(&registrations, &registration);
    }
    fclose(fp);
    return 0;
}

int saveClinicTime() {
    int i;
    FILE* fp = NULL;
    ClinicTime* clinicTime;
    if (fileOpen(&fp, "storage\\clinicTime.csv", "w")) {
        return -1;
    }
    for (i = 0; i < clinicTimes.length; i++) {
        clinicTime = getItem(&clinicTimes, i);
        fprintf(fp, "%u,%lld,%d,%02u%02u,%02u%02u,%u,%d\n", clinicTime->clinicTimeId, clinicTime->doctorId, clinicTime->weekday, clinicTime->startTime.hour,
            clinicTime->startTime.minute, clinicTime->endTime.hour, clinicTime->endTime.minute, clinicTime->maxAmount, clinicTime->status);
    }
    fclose(fp);
    return 0;
}

int saveClinicTimeRecord() {
    int i;
    FILE* fp = NULL;
    ClinicTimeRecord* record;
    if (fileOpen(&fp, "storage\\clinicTimeRecord.csv", "w")) {
        return -1;
    }
    for (i = 0; i < clinicTimeRecords.length; i++) {
        record = getItem(&clinicTimeRecords, i);
        fprintf(fp, "%u,%04u%02u%02u,%u\n", record->clinicTimeId, record->datetime.year, record->datetime.month,
            record->datetime.day, record->remainAmount);
    }
    fclose(fp);
    return 0;
}

int saveRegistrations() {
    int i;
    char datetime[15];
    FILE* fp = NULL;
    RegistrationRecord* registration;
    if (fileOpen(&fp, "storage\\registration.csv", "w")) {
        return -1;
    }
    for (i = 0; i < registrations.length; i++) {
        registration = getItem(&registrations, i);
        sprintf(datetime, "%04u%02u%02u%02u%02u", registration->datetime.year, registration->datetime.month,
            registration->datetime.day, registration->datetime.hour, registration->datetime.minute);
        fprintf(fp, "%u,%u,%s,%lld,%lld,%d\n", registration->registrationId, registration->clinicTimeId,
            datetime, registration->doctorId, registration->patientId, registration->status);
    }
    fclose(fp);
    return 0;
}

int addClinicTime(unsigned doctorId, Weekday weekday, Time startTime, Time endTime, unsigned maxAmount) {
    if (!clinicTimes.ptr) {
        loadClinicTime();
    }
    ClinicTime clinicTime = {
        clinicTimes.length,
        doctorId,
        weekday,
        startTime,
        endTime,
        maxAmount,
        valid
    };
    appendItem(&clinicTimes, &clinicTime);
    saveClinicTime();
    return clinicTime.clinicTimeId;
}

int setClinicTimeStatus(unsigned clinicTimeId, Status status) {
    if (!clinicTimes.ptr) {
        loadClinicTime();
    }
    ((ClinicTime*)getItem(&clinicTimes, clinicTimeId))->status = status;
    saveClinicTime();
    return 0;
}

ClinicTimeRecord* getClinicTimeRecord(unsigned clinicTimeId, Datetime datetime) {
    int i;
    ClinicTimeRecord* clinicTimeRecord;
    if (!clinicTimeRecords.ptr) {
        loadClinicTimeRecord();
    }
    for (i = 0; i < clinicTimeRecords.length; i++) {
        clinicTimeRecord = getItem(&clinicTimeRecords, i);
        if (clinicTimeRecord->clinicTimeId == clinicTimeId && clinicTimeRecord->datetime.year == datetime.year &&
            clinicTimeRecord->datetime.month == datetime.month && clinicTimeRecord->datetime.day == datetime.day) {
            return clinicTimeRecord;
        }
    }
    clinicTimeRecord = malloc(sizeof(ClinicTimeRecord));
    clinicTimeRecord->clinicTimeId = clinicTimeId;
    clinicTimeRecord->datetime.year = datetime.year;
    clinicTimeRecord->datetime.month = datetime.month;
    clinicTimeRecord->datetime.day = datetime.day;
    clinicTimeRecord->remainAmount = ((ClinicTime*)getItem(&clinicTimes, clinicTimeId))->maxAmount;
    appendItem(&clinicTimeRecords, clinicTimeRecord);
    free(clinicTimeRecord);
    return getItem(&clinicTimeRecords, clinicTimeRecords.length - 1);
}

unsigned selectClinicTime(Datetime datetime) {
    int i, availableTimesCount = 0, choice;
    char** title = NULL;

    if (!clinicTimes.ptr) {
        loadClinicTime();
    }
    if (!clinicTimeRecords.ptr) {
        loadClinicTimeRecord();
    }

    Weekday weekday = getWeekday(datetime);
    ClinicTime* clinicTime, ** availableTimes = calloc(clinicTimes.length, sizeof(ClinicTime*));
    ClinicTimeRecord* clinicTimeRecord;
    for (i = 0; i < clinicTimes.length; i++) {
        clinicTime = getItem(&clinicTimes, i);
        if (clinicTime->weekday == weekday && clinicTime->status == valid) {
            availableTimes[availableTimesCount] = clinicTime;
            availableTimesCount++;
        }
    }
    title = calloc(availableTimesCount, sizeof(char*));
    for (i = 0; i < availableTimesCount; i++) {
        clinicTime = availableTimes[i];
        clinicTimeRecord = getClinicTimeRecord(clinicTime->clinicTimeId, datetime);
        // TODO: getName(doctorId)
        title[i] = calloc(CLINIC_TIME_TITLE_LENGTH, sizeof(char));
        sprintf(title[i], "%sҽ�� %02u:%02u~%02u:%02u ��%02d", "����", clinicTime->startTime.hour, clinicTime->startTime.minute,
            clinicTime->endTime.hour, clinicTime->endTime.minute, clinicTimeRecord->remainAmount);
    }
    choice = displaySelect("ѡ��Һ�ʱ��Σ�", availableTimesCount, title);
    clinicTime = choice == -1 ? NULL : availableTimes[choice];
    free(availableTimes);
    free(title);
    return clinicTime ? clinicTime->clinicTimeId : -1;
}

void printRegistration(unsigned registrationId, short showTitle) {
    RegistrationRecord* registration = getItem(&registrations, registrationId);
    if (showTitle) {
        displayTitle("�Һ�����");
    }
    printf("�Һ� %010u %04u/%02u/%02u %02u:%02u\n", registration->registrationId, registration->datetime.year,
        registration->datetime.month, registration->datetime.day, registration->datetime.hour, registration->datetime.minute);
    /* TODO */
    // patient = getPatient(registration->patientId);
    printf("���ߣ�%s %s %d�� ���￨��%lld\n", "����", "��", 18, registration->patientId);
    // doctor = getDoctor(registration->doctorId);
    printf("ҽ����%s %s ����%lld\n", "����", "������ҽʦ", 100);
    return;
}

int appendRegistration(long long patientId, unsigned clinicTimeId, Datetime datetime) {
    int i;
    if (!clinicTimes.ptr) {
        loadClinicTime();
    }
    if (!clinicTimeRecords.ptr) {
        loadClinicTimeRecord();
    }
    if (!registrations.ptr) {
        loadRegistrations();
    }
    ClinicTime* clinicTime = getItem(&clinicTimes, clinicTimeId);
    ClinicTimeRecord* clinicTimeRecord = NULL;
    if (clinicTime->status == invalid) {
        printf(Red("����")"�Һ�ʱ��β����á�\n");
        return -1;
    }
    if (clinicTime->weekday != getWeekday(datetime)) {
        printf(Red("����")"���ڲ�ƥ�䡣\n");
        return -1;
    }
    clinicTimeRecord = getClinicTimeRecord(clinicTimeId, datetime);
    if (clinicTimeRecord->remainAmount > 0) {
        clinicTimeRecord->remainAmount--;
    } else {
        printf(Red("����")"��ʱ�ιҺ�������\n");
        return -1;
    }
    RegistrationRecord registration = {
        registrations.length,
        clinicTimeId,
        datetime,
        clinicTime->doctorId,
        patientId,
        valid
    };
    appendItem(&registrations, &registration);
    saveClinicTimeRecord();
    saveRegistrations();
    printf("�Һųɹ�����ĹҺ���Ϣ���£�\n");
    printRegistration(registrations.length - 1, 0);
    return 0;
}

int assignRegistration(unsigned patientId) {
    char temp[15], trash;
    unsigned clinicTimeId;
    Datetime datetime;
    displayInput("������Һ�����", "%s", temp);
    sscanf(temp, "%u%c%u%c%u", &datetime.year, &trash, &datetime.month, &trash, &datetime.day);
    printf("��ѡ��Һ�ҽ����ʱ�Σ�\n");
    clinicTimeId = selectClinicTime(datetime);
    if (clinicTimeId == -1) {
        return -1;
    }
    displayInput("������ԤԼʱ��", "%s", temp);
    sscanf(temp, "%u%c%u", &datetime.hour, &trash, &datetime.minute);
    appendRegistration(patientId, clinicTimeId, datetime);
}