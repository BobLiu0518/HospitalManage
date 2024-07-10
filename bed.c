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
        printf(Red("����")"�� storage\\BedInfo.txt ʧ�ܡ�\n");
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
        printf(Red("����")"�� storage\\BedInfo.txt ʧ�ܡ�\n");
        return;
    }
    if (zonesCount) {
        for (i = 0; i < zonesCount; i++) {
            free(zones[i].beds);
        }
        zonesCount = 0;
    }
    while (!feof(file)) {
        fscanf(file, "[%s %s %d]\n", zones[zonesCount].zoneName, zones[zonesCount].department, &zones[zonesCount].bedsCount);
        zones[zonesCount].beds = calloc(zones[zonesCount].bedsCount, sizeof(Bed));
        for (i = 0; i < zones[zonesCount].bedsCount; i++) {
            fscanf(file, "%d,%d\n", &zones[zonesCount].beds[i].bedNumber, &zones[zonesCount].beds[i].status);
        }
        zonesCount++;
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
        printf(Red("����")"�� storage\\BedRecord.txt ʧ�ܡ�\n");
        return;
    }
    for (i = 0; i < zonesCount; i++) {
        if (strcmp(zones[i].zoneName, zoneName) == 0) {
            if (zones[i].beds[bedNumber - 1].status == BED_STATUS_OCCUPIED) {
                printf(Red("����")"�ô�λ�ѱ�ռ�á�\n");
            }
            zones[i].beds[bedNumber - 1].status = BED_STATUS_OCCUPIED;
            fseek(file, 0, SEEK_SET);
            fprintf(file, "����%lld %04u/%02u/%02u %s����%d��\n",
                record.patientId, record.admissionDate.year, record.admissionDate.month, record.admissionDate.day,
                record.zoneName, record.bedNumber);
            fclose(file);
            saveZones();
            return;
        }
    }
    printf(Red("����")"���� %s �����ڡ�", zoneName);
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
        sprintf(zoneNames[i], "%s %s", zones[i].zoneName, zones[i].department);
    }
    *zoneIndex = displaySelect("ѡ������", zonesCount, zoneNames);
    for (i = 0; i < zonesCount; i++) {
        free(zoneNames[i]);
    }
    free(zoneNames);
    if (i == -1) {
        return -1;
    }
    printf("[%s] %s ��%d��λ (1~%d)\n", zones[*zoneIndex].zoneName, zones[*zoneIndex].department,
        zones[*zoneIndex].bedsCount, zones[*zoneIndex].bedsCount);
    displayInput("ѡ�񲡴�", "%d", bedNumber);
    if (*bedNumber < 1 || *bedNumber > zones[*zoneIndex].bedsCount) {
        printf(Red("����")"���������ڡ�\n");
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
            printf("�ɹ����ô�λ��\n");
            return 0;
        } else {
            printf("�ô�λ�ѱ�ռ�á�\n");
            return -1;
        }
    }
}

int deleteOccupancy(int zoneIndex, int bedNumber) {
    if (zonesCount == 0) {
        loadZones();
    }
    if (zones[zoneIndex].beds[bedNumber - 1].status == BED_STATUS_EMPTY) {
        printf(Red("����")"�ô�λδ��ռ�á�\n");
        return -1;
    }
    zones[zoneIndex].beds[bedNumber - 1].status = BED_STATUS_EMPTY;
    saveZones();
    return 0;
}

int bedMain() {
    loadZones();
    int choice;
    while (1) {
        choice = displaySelect("[����Ա] ��λ����", -4, "���ô�λռ��", "��ѯ��λռ�ü�¼", "ȡ����λռ��", "�˳���λ����");
        switch (choice) {
        case 0:
            long long patientId;
            USERS* patient;
            displayInput("���뻼��ID", "%lld", &patientId);
            patient = find_user_by_id(patientId);
            if (patient == NULL || patient->user_type != 2) {
                printf(Red("����")"���� %lld �����ڡ�", patientId);
            } else {
                setOccupy(patientId);
            }
            break;
        case 1:
            printf("��λռ�ü�¼��\n");
            FILE* file = fopen("storage\\BedRecord.txt", "r");
            if (file) {
                char line[256];
                while (fgets(line, sizeof(line), file)) {
                    printf("%s", line);
                }
                fclose(file);
            } else {
                printf("�޴�λռ�ü�¼��\n");
            }
            break;
        case 2:
            int zoneIndex, bedNumber;
            if (selectBed(&zoneIndex, &bedNumber) == 0) {
                if (deleteOccupancy(zoneIndex, bedNumber) == 0) {
                    printf("�����ɹ���\n");
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
