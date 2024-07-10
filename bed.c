#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "bed.h"
#include "beautifulDisplay.h"
#include "033.h"

Zone zones[MAX_ZONES];

void initializeZones() {
    for (int i = 0; i < MAX_ZONES; i++) {
        sprintf(zones[i].zoneName, "%02d区", i + 1);
        for (int j = 0; j < MAX_BEDS_PER_ZONE; j++) {
            zones[i].beds[j].bedNumber = j + 1;
            zones[i].beds[j].status = BED_STATUS_EMPTY;
        }
    }
}

void saveZonesToFile(const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        printf(Red("错误：")"打开 %s 失败。\n", filename);
        exit(-1);
    }
    for (int i = 0; i < MAX_ZONES; i++) {
        fprintf(file, "%s\n", zones[i].zoneName);
        for (int j = 0; j < MAX_BEDS_PER_ZONE; j++) {
            fprintf(file, "%d,%d\n", zones[i].beds[j].bedNumber, zones[i].beds[j].status);
        }
    }
    fclose(file);
}

void loadZonesFromFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf(Red("错误：")"打开 %s 失败。\n", filename);
        exit(-1);
    }
    char zoneName[50];
    int bedNumber, status;
    char line[100];
    for (int i = 0; i < MAX_ZONES && fgets(line, sizeof(line), file); i++) {
        sscanf(line, "%49[^\n]", zoneName);
        strcpy(zones[i].zoneName, zoneName);
        for (int j = 0; j < MAX_BEDS_PER_ZONE && fgets(line, sizeof(line), file); j++) {
            sscanf(line, "%d,%d", &bedNumber, &status);
            zones[i].beds[j].bedNumber = bedNumber;
            zones[i].beds[j].status = status;
        }
    }
    fclose(file);
}

void recordOccupancy(const char* patientName, const char* admissionDate, int zoneIndex, int bedNumber) {
    OccupancyRecord record;
    strcpy(record.patientName, patientName);
    strcpy(record.admissionDate, admissionDate);
    record.zoneIndex = zoneIndex;
    record.bedNumber = bedNumber;

    FILE* file = fopen("storage\\BedList.txt", "a");
    if (!file) {
        printf(Red("错误：")"打开 storage\\BedList.txt 失败。\n");
        exit(-1);
    }
    fprintf(file, "Patient: %s, Admission Date: %s, Zone: %d, Bed: %d\n",
        record.patientName, record.admissionDate, record.zoneIndex + 1, record.bedNumber);
    fclose(file);

    // Update bed status in memory
    zones[zoneIndex].beds[bedNumber - 1].status = BED_STATUS_OCCUPIED;
}


int deleteOccupancy(int zoneIndex, int bedNumber) {
    // 更新内存中的床位状态
    if (zoneIndex >= 0 && zoneIndex < MAX_ZONES && bedNumber >= 1 && bedNumber <= MAX_BEDS_PER_ZONE) {
        if (zones[zoneIndex].beds[bedNumber - 1].status == BED_STATUS_OCCUPIED) {
            zones[zoneIndex].beds[bedNumber - 1].status = BED_STATUS_EMPTY;
            printf("Bed %d in Zone %d is now marked as empty.\n", bedNumber, zoneIndex + 1);
            return 0; // 成功时返回0
        } else
            return -1; // 查无此人时返回-1
    } else {
        return -2; // 全失败时返回-2
    }
}

int bedMain() {
    FILE* testFile = fopen("storage\\BedInfo.txt", "r");
    if (testFile) {
        fclose(testFile);
        loadZonesFromFile("storage\\BedInfo.txt");
    } else {
        initializeZones();
        saveZonesToFile("storage\\BedInfo.txt");
    }

    int choice;
    do {
        choice = displaySelect("[管理员] 床位管理", -4, "设置床位占用", "查询床位状态", "取消床位占用", "退出床位管理");

        switch (choice) {
        case 0: {
            char patientName[100];
            char admissionDate[50];
            int zoneIndex, bedNumber;

            printf("Enter patient name: ");
            fgets(patientName, sizeof(patientName), stdin);
            patientName[strcspn(patientName, "\n")] = 0;

            printf("Enter admission date (e.g.2004-07-03): ");
            fgets(admissionDate, sizeof(admissionDate), stdin);
            admissionDate[strcspn(admissionDate, "\n")] = 0;

            printf("Select a zone:\n");
            for (int i = 0; i < MAX_ZONES; i++) {
                printf("%d. %s\n", i + 1, zones[i].zoneName);
            }
            printf("Enter zone index: ");
            scanf("%d", &zoneIndex);
            if (zoneIndex > MAX_ZONES || zoneIndex < 1) {
                printf("Invalid zone.( Numbers from 1-5 are acceptable.)\n");
                break;
            } else {
                getchar(); // Consume newline left in stdin buffer

                printf("Enter bed number: ");
                scanf("%d", &bedNumber);

                // Check if the bed is available
                if (bedNumber - 1 < MAX_BEDS_PER_ZONE && bedNumber >= 1) {
                    if (zones[zoneIndex - 1].beds[bedNumber - 1].status == BED_STATUS_EMPTY) {
                        recordOccupancy(patientName, admissionDate, zoneIndex - 1, bedNumber);
                        printf("Occupancy recorded successfully.\n");
                    } else {
                        printf("The selected bed is already occupied.\n");
                    }
                    break;
                } else
                    printf("Invalid bed.( Numbers from 1-10 are acceptable.)\n");
                break;
            }
        }
        case 1: {
            printf("Bed occupancy records:\n");
            FILE* recordsFile = fopen("storage\\BedList.txt", "r");
            if (recordsFile) {
                char line[256];
                while (fgets(line, sizeof(line), recordsFile)) {
                    printf("%s", line);
                }
                fclose(recordsFile);
            } else {
                printf("No records found.\n");
            }
            break;
        }
        case 2: {
            int zoneIndex, bedNumber;

            printf("Enter zone index to delete occupancy (1-%d): ", MAX_ZONES);
            scanf("%d", &zoneIndex);
            getchar();

            if (zoneIndex < 1 || zoneIndex > MAX_ZONES) {
                printf("Invalid zone index.\n");
                break;
            }

            printf("Enter bed number to delete occupancy (1-%d): ", MAX_BEDS_PER_ZONE);
            scanf("%d", &bedNumber);

            if (bedNumber < 1 || bedNumber > MAX_BEDS_PER_ZONE) {
                printf("Invalid bed number.\n");
                break;
            }

            if (deleteOccupancy(zoneIndex - 1, bedNumber) == -1) {
                printf("No existing occupancy there.\n");
            }
            break;
        }
        case -1:
        case 3:
            saveZonesToFile("storage\\BedInfo.txt");
            return 0;
        }
        system("pause > nul");
    } while (choice != 3);

    return 0;
}
