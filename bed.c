#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "bed.h"

Zone zones[MAX_ZONES];

void initializeZones() {
    for (int i = 0; i < MAX_ZONES; i++) {
        sprintf(zones[i].zoneName, "Zone %d", i + 1);
        for (int j = 0; j < MAX_BEDS_PER_ZONE; j++) {
            zones[i].beds[j].bedNumber = j + 1;
            zones[i].beds[j].status = BED_STATUS_EMPTY;
        }
    }
}

void saveZonesToFile(const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
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
        perror("Error opening file");
        exit(EXIT_FAILURE);
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

    FILE* file = fopen("BedList.txt", "a");
    if (!file) {
        perror("Error opening BedList.txt");
        exit(EXIT_FAILURE);
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
    // Load bed information from file if it exists
    FILE* testFile = fopen("BedInfo.txt", "r");
    if (testFile) {
        fclose(testFile);
        loadZonesFromFile("BedInfo.txt");
    } else {
        initializeZones();
        saveZonesToFile("BedInfo.txt");
    }

    // Main loop to interact with the user
    int choice;
    do {
        printf("\nWelcome to Bed Management System!\n");
        printf("1. Record a new occupancy\n");
        printf("2. View bed occupancy records\n");
        printf("3. Delete an existing occupancy\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // Consume newline left in stdin buffer

        switch (choice) {
        case 1: {
            char patientName[100];
            char admissionDate[50];
            int zoneIndex, bedNumber;

            printf("Enter patient name: ");
            fgets(patientName, sizeof(patientName), stdin);
            patientName[strcspn(patientName, "\n")] = 0; // Remove newline character

            printf("Enter admission date (e.g.2004-07-03): ");
            fgets(admissionDate, sizeof(admissionDate), stdin);
            admissionDate[strcspn(admissionDate, "\n")] = 0; // Remove newline character

            printf("Select a zone:\n");
            for (int i = 0; i < MAX_ZONES; i++) {
                printf("%d. %s\n", i + 1, zones[i].zoneName);
            }
            printf("Enter zone index: ");
            scanf("%d", &zoneIndex);
            if (zoneIndex > MAX_ZONES || zoneIndex < 1) {
                printf("Invaild zone.( Numbers from 1-5 are acceptable.)\n");
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
                    printf("Invaild bed.( Numbers from 1-10 are acceptable.)\n");
                break;
            }
        }
        case 2: {
            printf("Bed occupancy records:\n");
            FILE* recordsFile = fopen("BedList.txt", "r");
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
        case 3: {
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
                printf("No exisiting occupancy there.\n");
            }
            break;
        }
        case 4:
            printf("Exiting...\n");
            break;
        default:
            printf("Invalid choice. Please try again.\n");

        }
        setbuf(stdin, NULL);
    } while (choice != 4);

    // Save the current bed statuses before exiting
    saveZonesToFile("BedInfo.txt");

    return 0;
}
