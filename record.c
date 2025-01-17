#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "beautifulDisplay.h"
#include "dynamicArray.h"
#include "fileTools.h"
#include "timeTools.h"
#include "033.h"
#include "record.h"
#include "user.h"
#include "bed.h"
#include "medicine.h"

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
    int i;
    char medicineAbbr[20], description[RECORD_CONTENT_LENGTH];
    Record newRecord;
    USERS* patient;
    Medicine* medicine;
    Datetime now = getDateTime();
    displayTitle("新增病历");
    if (!records.length) {
        loadRecordData();
    }

    newRecord.recordId = ((Record*)getItem(&records, records.length - 1))->recordId + 1;
    newRecord.datetime = getDateTime();
    newRecord.doctorId = doctorId;
    displayInput("请输入患者就诊卡号", "%lld", &newRecord.patientId);
    patient = find_user_by_id(newRecord.patientId);
    if (!patient || patient->user_type != 2) {
        printf(Red("错误：")"患者 %lld 不存在。\n", newRecord.patientId);
        return -1;
    }
    printf("患者：%s %s %d岁\n", patient->name, patient->sex, now.year - patient->birth.year);
    displayInputMultiline("请输入病历信息", newRecord.content, RECORD_CONTENT_LENGTH);
    while (1) {
        i = displaySelect("是否需要添加用药？", -2, "是", "否");
        if (i) {
            break;
        }
        displayTitle("添加用药");
        displayInput("请输入药物缩写", "%s", medicineAbbr);
        displayInput("请输入药物用量", "%d", &i);
        if (!ModifyStock(medicineAbbr, i)) {
            medicine = getMedicine(medicineAbbr);
            sprintf(description, "\n【用药】%s[%s] %d单位", medicine->fullName, medicineAbbr, i);
            strcat(newRecord.content, description);
            printf("添加药物成功。\n");
        }
        system("pause > nul");
    }
    i = displaySelect("是否需要设置住院？", -2, "是", "否");
    if (!i) {
        if (!setOccupy(patient->id)) {
            strcat(newRecord.content, "\n【住院】");
        }
        system("pause > nul");
    }
    appendItem(&records, &newRecord);
    saveRecordData();
    printRecord(((Record*)getItem(&records, records.length - 1))->recordId);
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
            displayTitle("病历详情");
            printf(Strong("病历 #%010lu %04u/%02u/%02u %02u:%02u\n"), recordId, record->datetime.year,
                record->datetime.month, record->datetime.day, record->datetime.hour, record->datetime.minute);
            patient = find_user_by_id(record->patientId);
            doctor = find_user_by_id(record->doctorId);
            printf("患者：%s %s %d岁 就诊卡号%lld\n", patient->name, patient->sex, now.year - patient->birth.year, record->patientId);
            printf("医生：%s %s %s 工号%lld\n", doctor->name, doctor->department, doctor->title, doctor->id);
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
    printf("未找到编号为 %010lu 的病历\n", recordId);
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
        printf("没有找到您的历史病历…");
        return -1;
    }

    recordTitle = calloc(historyRecordCount + 1, sizeof(char*));
    for (i = 0; i < historyRecordCount; i++) {
        recordTitle[i] = calloc(RECORD_TITLE_LENGTH, sizeof(char));
        sprintf(recordTitle[i], "病历 #%010lu %04u/%02u/%02u %02u:%02u", historyRecord[i]->recordId,
            historyRecord[i]->datetime.year, historyRecord[i]->datetime.month, historyRecord[i]->datetime.day,
            historyRecord[i]->datetime.hour, historyRecord[i]->datetime.minute);
    }
    recordTitle[historyRecordCount] = "退出病历查询";

    while (1) {
        selection = displaySelect("请选择要查看的病历：", historyRecordCount + 1, recordTitle);
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