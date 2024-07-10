#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bed.h"
#include "user.h"
#include "beautifulDisplay.h"
#include "timeTools.h"
#include "033.h"

Zone zones[MAX_ZONES];
int zonesCount = 0;

void saveZones() {
    FILE* file = fopen("storage\\BedInfo.txt", "w");
    if (!file) {
        printf(Red("错误：")"打开 storage\\BedInfo.txt 失败。\n");
        return;
    }
    for (int i = 0; i < zonesCount; i++) {
        fprintf(file, "[%s %s %d]\n", zones[i].zoneName, zones[i].department, zones[i].bedsCount);
        for (int j = 0; j < zones[i].bedsCount; j++) {
            fprintf(file, "%d,%d\n", zones[i].beds[j].bedNumber, zones[i].beds[j].status);
        }
    }
    fclose(file);
}

void loadZones() {
    int i;
    FILE* file = fopen("storage\\BedInfo.txt", "r");
    if (!file) {
        printf(Red("错误：")"打开 storage\\BedInfo.txt 失败。\n");
        return;
    }
    if (zonesCount) {
        // TODO: free
        zonesCount = 0;
    }
    while (!feof(file)) {
        fscanf(file, "[%s %s %d]\n", zones[zonesCount].zoneName, zones[zonesCount].department, zones[zonesCount].bedsCount);
        zones[zonesCount].beds = calloc(zones[zonesCount].bedsCount, sizeof(Bed));
        for (i = 0; i < zones[zonesCount].bedsCount; i++) {
            fscanf(file, "%d,%d\n", zones[zonesCount].beds[i].bedNumber, zones[zonesCount].beds[i].status);
        }
    }
    fclose(file);
}

void recordOccupancy(long long patientId, Datetime admissionDate, char* zoneName, int bedNumber) {
    int i;
    OccupancyRecord record;
    if (zonesCount == 0) {
        loadZones();
    }
    record.patientId = patientId;
    record.admissionDate = admissionDate;
    strcpy(record.zoneName, zoneName);
    record.bedNumber = bedNumber;

    FILE* file = fopen("storage\\BedRecord.txt", "a");
    if (!file) {
        printf(Red("错误：")"打开 storage\\BedRecord.txt 失败。\n");
        return;
    }
    for (i = 0; i < zonesCount; i++) {
        if (strcmp(zones[i].zoneName, zoneName) == 0) {
            if (zones[i].beds[bedNumber - 1].status == BED_STATUS_OCCUPIED) {
                printf(Red("错误：")"该床位已被占用。\n");
            }
            zones[i].beds[bedNumber - 1].status = BED_STATUS_OCCUPIED;
            fprintf(file, "Patient: %d, Admission Date: %04u/%02u/%02u, Zone: %s, Bed: %d\n",
                record.patientId, record.admissionDate.year, record.admissionDate.month, record.admissionDate.day,
                record.zoneName, record.bedNumber);
            fclose(file);
            saveZones();
            return;
        }
    }
    printf(Red("错误：")"病区 %s 不存在。", zoneName);
    fclose(file);
}

int selectBed(int* zoneIndex, int* bedNumber) {
    int i;
    if (!zonesCount) {
        loadZones();
    }
    char** zoneNames = calloc(zonesCount, sizeof(char*));
    for (i = 0; i < zonesCount; i++) {
        zoneNames[i] = calloc(40, sizeof(char));
        sprintf(zoneNames[i], "[%s] %s", zones[i].zoneName, zones[i].department);
    }
    *zoneIndex = displaySelect("选择病区：", zonesCount);
    for (i = 0; i < zonesCount; i++) {
        free(zoneNames[i]);
    }
    free(zoneNames);
    if (i == -1) {
        return -1;
    }
    printf("[%s] %s 共%d床位 (1~%d)\n", zones[*zoneIndex].zoneName, zones[*zoneIndex].department,
        zones[*zoneIndex].bedsCount, zones[*zoneIndex].bedsCount);
    displayInput("选择病床", "%d", bedNumber);
    if (*bedNumber < 1 || *bedNumber > zones[i].bedsCount) {
        printf(Red("错误：")"病床不存在。\n");
        return -1;
    }
    return 0;
}

int setOccupy(long long patientId) {
    int zoneIndex, bedNumber;
    Datetime now = getDateTime();
    if (selectBed(&zoneIndex, &bedNumber) == 0) {
        if (zones[zoneIndex].beds[bedNumber - 1].status == BED_STATUS_EMPTY) {
            recordOccupancy(patientId, now, zones[zoneIndex].zoneName, bedNumber);
            printf("成功设置床位。\n");
        } else {
            printf("该床位已被占用。\n");
            return -1;
        }
    }
    return 0;
}

int deleteOccupancy(int zoneIndex, int bedNumber) {
    if (zonesCount == 0) {
        loadZones();
    }
    if (zones[zoneIndex].beds[bedNumber - 1].status == BED_STATUS_EMPTY) {
        printf(Red("错误：")"该床位已被占用。\n");
        return -1;
    }
    zones[zoneIndex].beds[bedNumber - 1].status = BED_STATUS_EMPTY;
    saveZones();
    return 0;
}

int bedMain() {
    printf("1111111");
    loadZones();
    printf("222222");
    int choice;
    while (1) {
        choice = displaySelect("[管理员] 床位管理", -4, "设置床位占用", "查询床位占用记录", "取消床位占用", "退出床位管理");
        switch (choice) {
        case 0:
            long long patientId;
            USERS* patient;
            displayInput("输入患者ID", "%lld", &patientId);
            patient = find_user_by_id(patientId);
            if (patient == NULL) {
                printf(Red("错误：")"患者 %lld 不存在。", patientId);
            } else {
                setOccupy(patientId);
            }
            break;
        case 1:
            printf("床位占用记录：\n");
            FILE* file = fopen("storage\\BedRecord.txt", "r");
            if (file) {
                char line[256];
                while (fgets(line, sizeof(line), file)) {
                    printf("%s", line);
                }
                fclose(file);
            } else {
                printf("无床位占用记录。\n");
            }
            break;
        case 2:
            int zoneIndex, bedNumber;
            if (selectBed(&zoneIndex, &bedNumber) == 0) {
                if (deleteOccupancy(zoneIndex, bedNumber) == -1) {
                    printf("No existing occupancy there.\n");
                }
            }
            break;
        case -1:
        case 3:
            return 0;
        }
        system("pause > nul");
    }
}
