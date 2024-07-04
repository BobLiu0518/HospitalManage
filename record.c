#include <stdio.h>
#include <stdlib.h>
#include "beautifulDisplay.h"
#include "consts.h"
#include "033.h"

typedef struct datetime {
    unsigned year;
    unsigned month;
    unsigned day;
    unsigned hour;
    unsigned minute;
} Datetime;

typedef struct record {
    unsigned long recordId;
    Datetime datetime;
    unsigned long long patientId;
    unsigned long long doctorId;
    char content[RECORD_CONTENT_LENGTH];
} Record;

Record record[RECORD_MAX];
int recordCount;

int loadRecordData() {
    int length;
    char filename[RECORD_FILENAME_LENGTH], datetime[15];
    FILE* fpRecord = fopen("storage\\record.csv", "r"), * fpContent = NULL;
    if (!fpRecord) {
        printf(Yellow("���棺")"��ȡ�ļ� storage\\record.csv ʧ�ܣ����Դ����հ��ļ���\n");
        fpRecord = fopen("storage\\record.csv", "w+");
        if (!fpRecord) {
            printf(Red("����")"�����ļ� storage\\record.csv ʧ�ܡ�\n");
            return -1;
        }
    }
    recordCount = 0;
    while (!feof(fpRecord)) {
        fscanf(fpRecord, "%lu,%[^,],%llu,%llu\n", &record[recordCount].recordId, datetime, &record[recordCount].patientId, &record[recordCount].doctorId);
        sscanf(datetime, "%4u%2u%2u%2u%2u", &record[recordCount].datetime.year, &record[recordCount].datetime.month,
            &record[recordCount].datetime.day, &record[recordCount].datetime.hour, &record[recordCount].datetime.minute);
        sprintf(filename, "storage\\record\\%010lu.txt", record[recordCount].recordId);
        fpContent = fopen(filename, "r");
        if (!fpContent) {
            printf(Yellow("���棺")"��ȡ�ļ� %s ʧ�ܣ����Դ����հ��ļ���\n", filename);
            fpContent = fopen(filename, "w+");
            if (!fpContent) {
                printf(Red("����")"�����ļ� %s ʧ�ܡ�\n", filename);
                record[recordCount].content[0] = '\0';
                recordCount++;
                continue;
            }
        }
        fseek(fpContent, 0, SEEK_END);
        length = ftell(fpContent);
        fseek(fpContent, 0, SEEK_SET);
        if (length) {
            fread(record[recordCount].content, 1, length, fpContent);
        }
        record[recordCount].content[length] = '\0';
        fclose(fpContent);
        recordCount++;
    }
    fclose(fpRecord);
    return 0;
}

int saveRecordData() {
    int i;
    char filename[RECORD_FILENAME_LENGTH], datetime[15];
    FILE* fpRecord = fopen("storage\\record.csv", "w"), * fpContent = NULL;
    if (!fpRecord) {
        printf(Red("����")"�����ļ� storage\\record.csv ʧ�ܡ�\n");
        return -1;
    }
    for (i = 0; i < recordCount && i < RECORD_MAX; i++) {
        sprintf(datetime, "%04u%02u%02u%02u%02u", record[i].datetime.year, record[i].datetime.month,
            record[i].datetime.day, record[i].datetime.hour, record[i].datetime.minute);
        fprintf(fpRecord, "%lu,%s,%llu,%llu\n", record[i].recordId, datetime, record[i].patientId, record[i].doctorId);
        sprintf(filename, "storage\\record\\%010lu.txt", record[i].recordId);
        fpContent = fopen(filename, "w");
        if (!fpContent) {
            printf(Red("����")"�����ļ� %s ʧ�ܡ�\n", filename);
            continue;
        }
        fprintf(fpContent, "%s", record[i].content);
        fclose(fpContent);
    }
    fclose(fpRecord);
    return 0;
}

int appendRecord(unsigned long long doctorId) {
    system("cls");
    if (!recordCount) {
        loadRecordData();
    }
    if (recordCount == RECORD_MAX) {
        printf(Red("����")"�������Ѵ����ޣ��޷�������ӡ�\n");
        return -1;
    }

    record[recordCount].recordId = record[recordCount - 1].recordId + 1;
    // TODO: datetime
    displayInput("�����뻼�� ID", "%llu", &record[recordCount].patientId);
    /* TODO */
    // patient = getPatient(record[recordCount].patientId);
    printf("���ߣ�%s %s %d��\n", "����", "��", 18);
    displayInputMultiline("�����벡����Ϣ", record[recordCount].content, RECORD_CONTENT_LENGTH);
    recordCount++;

    saveRecordData();
    printf("��Ӳ�����Ϣ�ɹ�\n");
    system("pause > nul");
    return 0;
}

int printRecord(unsigned long recordId) {
    int i, j;
    if (!recordCount) {
        loadRecordData();
    }
    for (i = 0; i < recordCount; i++) {
        if (record[i].recordId == recordId) {
            printf(Strong("���� #%010lu %04u/%02u/%02u %02u:%02u\n"), recordId, record[i].datetime.year,
                record[i].datetime.month, record[i].datetime.day, record[i].datetime.hour, record[i].datetime.minute);
            /* TODO */
            // patient = getPatient(record[i].patientId);
            printf("���ߣ�%s %s %d�� ���￨��%llu\n", "����", "��", 18, record[i].patientId);
            // doctor = getDoctor(record[i].doctorId);
            printf("ҽ����%s %s ����%llu\n", "����", "������ҽʦ", 100);
            for (j = 0; j < 40; j++) {
                printf("-");
            }
            printf("\n%s\n", record[i].content);
            for (j = 0; j < 40; j++) {
                printf("-");
            }
            printf("\n");
            system("pause > nul");
            return 0;
        }
    }
    printf("δ�ҵ����Ϊ %010lu �Ĳ���\n", recordId);
    system("pause > nul");
    return -1;
}

int checkHistoryRecord(unsigned long long patientId) {
    if (!recordCount) {
        loadRecordData();
    }
}

int main() {
    system("chcp 936 > nul");
    loadRecordData();
    printRecord(1);
    appendRecord(114514);
    system("pause > nul");
}