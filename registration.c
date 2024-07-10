#include <stdio.h>
#include <stdlib.h>
#include "beautifulDisplay.h"
#include "dynamicArray.h"
#include "fileTools.h"
#include "timeTools.h"
#include "033.h"
#include "user.h"
#include "registration.h"

DynamicArray clinicTimes = { 0 };
DynamicArray registrations = { 0 };
DynamicArray clinicTimeRecords = { 0 };
char* weekdayName[ ] = { "����", "��һ", "�ܶ�", "����", "����", "����", "����" };

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
        c = fscanf(fp, "%u,%lld,%d,%2u%2u,%2u%2u,%u,%d\n", &clinicTime.clinicTimeId, &clinicTime.doctorId, &clinicTime.weekday,
            &clinicTime.startTime.hour, &clinicTime.startTime.minute,
            &clinicTime.endTime.hour, &clinicTime.endTime.minute, &clinicTime.maxAmount, &clinicTime.status);
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
        fprintf(fp, "%u,%lld,%d,%02u%02u,%02u%02u,%u,%d\n", clinicTime->clinicTimeId, clinicTime->doctorId, clinicTime->weekday,
            clinicTime->startTime.hour, clinicTime->startTime.minute, clinicTime->endTime.hour, clinicTime->endTime.minute,
            clinicTime->maxAmount, clinicTime->status);
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

unsigned addClinicTime(long long doctorId) {
    if (!clinicTimes.ptr) {
        loadClinicTime();
    }
    Weekday weekday;
    Time startTime, endTime;
    unsigned maxAmount;
    char temp[15], trash;

    weekday = displaySelect("ѡ��������", 7, weekdayName);
    if (weekday == -1) {
        return -1;
    }
    displayInput("�����뿪ʼʱ��", "%s", temp);
    sscanf(temp, "%u%c%u", &startTime.hour, &trash, &startTime.minute);
    displayInput("���������ʱ��", "%s", temp);
    sscanf(temp, "%u%c%u", &endTime.hour, &trash, &endTime.minute);
    displayInput("�������������", "%u", &maxAmount);

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
    printf("�������ʱ��ɹ���\n");
    system("pause > nul");
    return clinicTime.clinicTimeId;
}

int editClinicTime(unsigned clinicTimeId, char* title) {
    if (!clinicTimes.ptr) {
        loadClinicTime();
    }
    int selection;
    ClinicTime* clinicTime = getItem(&clinicTimes, clinicTimeId);
    selection = displaySelect(title, -3, "���Ľ�������", clinicTime->status == valid ? "����Ϊ�ر�" : "����Ϊ����", "ȡ��");
    if (selection == 0) {
        displayInput("�������������", "%u", &clinicTime->maxAmount);
        printf("�޸ĳɹ���\n");
        system("pause > nul");
    } else if (selection == 1) {
        clinicTime->status = !clinicTime->status;
        printf("���óɹ���\n");
        system("pause > nul");
    }
    saveClinicTime();
}

int manageClinicTime(long long doctorId) {
    int i, docClinicTimeCount = 0;
    char** title;
    if (!clinicTimes.ptr) {
        loadClinicTime();
    }
    ClinicTime** docClinicTime = calloc(clinicTimes.length, sizeof(ClinicTime*)), * clinicTime;
    for (i = 0; i < clinicTimes.length; i++) {
        clinicTime = getItem(&clinicTimes, i);
        if (clinicTime->doctorId == doctorId) {
            docClinicTime[docClinicTimeCount] = clinicTime;
            docClinicTimeCount++;
        }
    }
    title = calloc(docClinicTimeCount + 2, sizeof(char*));
    for (i = 0; i < docClinicTimeCount; i++) {
        title[i] = calloc(CLINIC_TIME_TITLE_LENGTH, sizeof(char));
        sprintf(title[i], "%s %02u:%02u~%02u:%02u ��%03u %s", weekdayName[docClinicTime[i]->weekday],
            docClinicTime[i]->startTime.hour, docClinicTime[i]->startTime.minute,
            docClinicTime[i]->endTime.hour, docClinicTime[i]->endTime.minute,
            docClinicTime[i]->maxAmount, docClinicTime[i]->status == valid ? "����" : "�ر�");
    }
    title[docClinicTimeCount] = "��������ʱ��";
    title[docClinicTimeCount + 1] = "�˳�����ʱ�����";
    i = displaySelect("ѡ������ʱ��", docClinicTimeCount + 2, title);
    if (i == docClinicTimeCount) {
        addClinicTime(doctorId);
    } else if (i != -1 && i != docClinicTimeCount + 1) {
        editClinicTime(docClinicTime[i]->clinicTimeId, title[i]);
    }

    for (i = 0; i < docClinicTimeCount; i++) {
        free(title[i]);
    }
    free(title);
    free(docClinicTime);
    return 0;
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

unsigned chooseClinicTime(Datetime datetime) {
    int i, availableTimesCount = 0, choice;
    char** title = NULL;
    USERS* doctor;

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
        clinicTimeRecord = getClinicTimeRecord(clinicTime->clinicTimeId, datetime);
        if (clinicTime->weekday == weekday && clinicTime->status == valid && clinicTimeRecord->remainAmount > 0) {
            availableTimes[availableTimesCount] = clinicTime;
            availableTimesCount++;
        }
    }
    title = calloc(availableTimesCount, sizeof(char*));
    for (i = 0; i < availableTimesCount; i++) {
        clinicTime = availableTimes[i];
        clinicTimeRecord = getClinicTimeRecord(clinicTime->clinicTimeId, datetime);
        doctor = find_user_by_id(clinicTime->doctorId);
        title[i] = calloc(CLINIC_TIME_TITLE_LENGTH, sizeof(char));
        sprintf(title[i], "%s %s%s %02u:%02u~%02u:%02u", doctor->department, doctor->name,
            doctor->title, clinicTime->startTime.hour, clinicTime->startTime.minute,
            clinicTime->endTime.hour, clinicTime->endTime.minute);
    }
    choice = displaySelect("ѡ��Һ�ʱ��Σ�", availableTimesCount, title);
    clinicTime = choice == -1 ? NULL : availableTimes[choice];

    for (i = 0; i < availableTimesCount; i++) {
        free(title[i]);
    }
    free(title);
    free(availableTimes);
    return clinicTime ? clinicTime->clinicTimeId : -1;
}

void printRegistration(unsigned registrationId, short showTitle) {
    RegistrationRecord* registration = getItem(&registrations, registrationId);
    USERS* patient, * doctor;
    Datetime now = getDateTime();
    if (showTitle) {
        displayTitle("�Һ�����");
    }
    printf("�Һ� #%010u %04u/%02u/%02u %02u:%02u %s\n", registration->registrationId,
        registration->datetime.year, registration->datetime.month, registration->datetime.day,
        registration->datetime.hour, registration->datetime.minute, registration->status == valid ? "" : "��ȡ��");
    patient = find_user_by_id(registration->patientId);
    doctor = find_user_by_id(registration->doctorId);
    printf("���ߣ�%s %s %d�� ���￨��%lld\n", patient->name, patient->sex, now.year - patient->birth.year, registration->patientId);
    printf("ҽ����%s %s %s ����%lld\n", doctor->name, doctor->department, doctor->title, registration->doctorId);
    return;
}

int editRegistrationRecord(unsigned registrationId) {
    int selection;
    if (!registrations.ptr) {
        loadRegistrations();
    }
    RegistrationRecord* registration = getItem(&registrations, registrationId);
    printRegistration(registrationId, 1);
    system("pause > nul");
    if (registration->status == valid) {
        selection = displaySelect("�Ƿ�ȡ���Һţ�", -2, "��", "��");
        if (!selection) {
            registration->status = invalid;
            saveRegistrations();
            printf("ȡ���Һųɹ���\n");
            system("pause > nul");
        }
    }
    return 0;
}

int checkRegistrationRecord(long long patientId) {
    int i, patientRegistrationCount = 0;
    char** title;
    USERS* doctor;
    if (!registrations.length) {
        loadRegistrations();
    }
    RegistrationRecord** patientRegistration = calloc(registrations.length, sizeof(RegistrationRecord*)), * registration;
    for (i = 0; i < registrations.length; i++) {
        registration = getItem(&registrations, i);
        if (registration->patientId == patientId || patientId == -1) {
            patientRegistration[patientRegistrationCount] = registration;
            patientRegistrationCount++;
        }
    }
    title = calloc(patientRegistrationCount + 1, sizeof(char*));
    for (i = 0; i < patientRegistrationCount; i++) {
        title[i] = calloc(REGISTRATION_TITLE_LENGTH, sizeof(char));
        doctor = find_user_by_id(patientRegistration[i]->doctorId);
        sprintf(title[i], "%04u/%02u/%02u %02u:%02u %s", patientRegistration[i]->datetime.year,
            patientRegistration[i]->datetime.month, patientRegistration[i]->datetime.day,
            patientRegistration[i]->datetime.hour, patientRegistration[i]->datetime.minute, doctor->name);
    }
    title[patientRegistrationCount] = "�˳��Һż�¼��ѯ";
    while (1) {
        i = displaySelect("��ѡ��Ҫ�鿴�ĹҺż�¼��", patientRegistrationCount + 1, title);
        if (i == -1 || i == patientRegistrationCount) {
            break;
        }
        if (patientId == -1) {
            editRegistrationRecord(patientRegistration[i]->registrationId);
        } else {
            printRegistration(patientRegistration[i]->registrationId, 1);
            system("pause > nul");
        }
    }
    for (i = 0; i < patientRegistrationCount; i++) {
        free(title[i]);
    }
    free(title);
    free(patientRegistration);
    return 0;
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
    return 0;
}

int assignRegistration(unsigned patientId) {
    char temp[15], trash;
    unsigned clinicTimeId;
    Datetime datetime;
    displayInput("������Һ�����", "%s", temp);
    sscanf(temp, "%u%c%u%c%u", &datetime.year, &trash, &datetime.month, &trash, &datetime.day);
    printf("��ѡ��Һ�ҽ����ʱ�Σ�\n");
    clinicTimeId = chooseClinicTime(datetime);
    if (clinicTimeId == -1) {
        return -1;
    }
    displayInput("������ԤԼʱ��", "%s", temp);
    sscanf(temp, "%u%c%u", &datetime.hour, &trash, &datetime.minute);
    appendRegistration(patientId, clinicTimeId, datetime);
    printf("�Һųɹ�����ĹҺ���Ϣ���£�\n");
    printRegistration(registrations.length - 1, 0);
    system("pause > nul");
}