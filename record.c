#include <stdio.h>
#include <stdlib.h>
#include "beautifulDisplay.h"
#include "dynamicArray.h"
#include "fileTools.h"
#include "timeTools.h"
#include "033.h"
#include "record.h"
#include "user.h"

DynamicArray records = { 0 };

int loadRecordData() {
    int length;
    char filename[RECORD_FILENAME_LENGTH], datetime[15];
    Record newRecord;
    FILE* fpRecord = NULL, * fpContent = NULL;
    if (fileOpen(&fpRecord, "storage\\record.csv", "r")) {
        return -1;
    }
    if (records.ptr) {
        freeDynamicArray(&records);
    }
    initDynamicArray(&records, 128, sizeof(Record));
    while (!feof(fpRecord)) {
        length = fscanf(fpRecord, "%lu,%[^,],%lld,%lld\n", &newRecord.recordId, datetime, &newRecord.patientId, &newRecord.doctorId);
        if (length == EOF) {
            break;
        }
        sscanf(datetime, "%4u%2u%2u%2u%2u", &newRecord.datetime.year, &newRecord.datetime.month,
            &newRecord.datetime.day, &newRecord.datetime.hour, &newRecord.datetime.minute);
        sprintf(filename, "storage\\record\\%010lu.txt", newRecord.recordId);
        if (fileOpen(&fpContent, filename, "r")) {
            newRecord.content[0] = '\0';
            appendItem(&records, &newRecord);
            continue;
        }
        fseek(fpContent, 0, SEEK_END);
        length = ftell(fpContent);
        rewind(fpContent);
        if (length) {
            fread(newRecord.content, 1, length, fpContent);
        }
        newRecord.content[length] = '\0';
        fclose(fpContent);
        appendItem(&records, &newRecord);
    }
    fclose(fpRecord);
    return 0;
}

int saveRecordData() {
    int i;
    char filename[RECORD_FILENAME_LENGTH], datetime[15];
    Record* record = NULL;
    FILE* fpRecord = NULL, * fpContent = NULL;
    if (fileOpen(&fpRecord, "storage\\record.csv", "w")) {
        return -1;
    }
    for (i = 0; i < records.length; i++) {
        record = getItem(&records, i);
        sprintf(datetime, "%04u%02u%02u%02u%02u", record->datetime.year, record->datetime.month,
            record->datetime.day, record->datetime.hour, record->datetime.minute);
        fprintf(fpRecord, "%lu,%s,%lld,%lld\n", record->recordId, datetime, record->patientId, record->doctorId);
        sprintf(filename, "storage\\record\\%010lu.txt", record->recordId);
        if (fileOpen(&fpContent, filename, "w")) {
            continue;
        }
        fprintf(fpContent, "%s", record->content);
        fclose(fpContent);
    }
    fclose(fpRecord);
    return 0;
}

int appendRecord(long long doctorId) {
    Record newRecord;
    USERS* patient;
    Datetime now = getDateTime();
    displayTitle("��������");
    if (!records.length) {
        loadRecordData();
    }

    newRecord.recordId = ((Record*)getItem(&records, records.length - 1))->recordId + 1;
    newRecord.datetime = getDateTime();
    newRecord.doctorId = doctorId;
    displayInput("�����뻼�� ID", "%lld", &newRecord.patientId);
    patient = find_user_by_id(newRecord.patientId);
    if (!patient || patient->user_type != 2) {
        printf(Red("����")"���� %lld �����ڡ�\n", newRecord.patientId);
        return -1;
    }
    printf("���ߣ�%s %s %d��\n", patient->name, patient->sex, now.year - patient->birth.year);
    displayInputMultiline("�����벡����Ϣ", newRecord.content, RECORD_CONTENT_LENGTH);
    // TODO����ҩ������
    appendItem(&records, &newRecord);
    saveRecordData();
    printf("��Ӳ�����Ϣ�ɹ�\n");
    system("pause > nul");
    return 0;
}

int printRecord(unsigned long recordId) {
    int i, j;
    Record* record;
    USERS* patient, * doctor;
    Datetime now = getDateTime();
    if (!records.length) {
        loadRecordData();
    }
    for (i = 0; i < records.length; i++) {
        record = getItem(&records, i);
        if (record->recordId == recordId) {
            displayTitle("��������");
            printf(Strong("���� #%010lu %04u/%02u/%02u %02u:%02u\n"), recordId, record->datetime.year,
                record->datetime.month, record->datetime.day, record->datetime.hour, record->datetime.minute);
            patient = find_user_by_id(record->patientId);
            doctor = find_user_by_id(record->doctorId);
            printf("���ߣ�%s %s %d�� ���￨��%lld\n", patient->name, patient->sex, now.year - patient->birth.year, record->patientId);
            printf("ҽ����%s %s %s ����%lld\n", doctor->name, doctor->department, doctor->title, doctor->id);
            for (j = 0; j < 40; j++) {
                printf("-");
            }
            printf("\n%s\n", record->content);
            for (j = 0; j < 40; j++) {
                printf("-");
            }
            printf("\n");
            return 0;
        }
    }
    printf("δ�ҵ����Ϊ %010lu �Ĳ���\n", recordId);
    return -1;
}

int checkHistoryRecord(long long patientId) {
    int historyRecordCount = 0, i, selection;
    char** recordTitle;
    Record** historyRecord = calloc(records.length, sizeof(Record*)), * record;
    if (!records.length) {
        loadRecordData();
    }
    for (i = 0; i < records.length; i++) {
        record = getItem(&records, i);
        if (patientId == record->patientId) {
            historyRecord[historyRecordCount] = record;
            historyRecordCount++;
        }
    }
    if (!historyRecordCount) {
        printf("û���ҵ�������ʷ������");
        return -1;
    }

    recordTitle = calloc(historyRecordCount + 1, sizeof(char*));
    for (i = 0; i < historyRecordCount; i++) {
        recordTitle[i] = calloc(RECORD_TITLE_LENGTH, sizeof(char));
        sprintf(recordTitle[i], "���� #%010lu %04u/%02u/%02u %02u:%02u", historyRecord[i]->recordId,
            historyRecord[i]->datetime.year, historyRecord[i]->datetime.month, historyRecord[i]->datetime.day,
            historyRecord[i]->datetime.hour, historyRecord[i]->datetime.minute);
    }
    recordTitle[historyRecordCount] = "�˳�������ѯ";

    while (1) {
        selection = displaySelect("��ѡ��Ҫ�鿴�Ĳ�����", historyRecordCount + 1, recordTitle);
        if (selection == -1 || selection == historyRecordCount) {
            break;
        }
        printRecord(historyRecord[selection]->recordId);
        system("pause > nul");
    }

    for (i = 0; i < historyRecordCount; i++) {
        free(recordTitle[i]);
    }
    free(recordTitle);
    free(historyRecord);
}