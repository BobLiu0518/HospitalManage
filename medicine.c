#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "033.h"
#include "medicine.h"
#include "beautifulDisplay.h"

Medicine* medicineHead = NULL;

void freeMedicine() {
    Medicine* current = medicineHead, * temp;
    while (current != NULL) {
        temp = current;
        current = current->next;
        free(temp);
    }
}

void exportMedicine() {
    FILE* fp = fopen("storage\\MedicineList.txt", "w");
    if (fp == NULL) {
        printf(Red("����")"�� storage\\MedicineList.txt ʧ�ܡ�\n");
        return;
    }

    Medicine* current = medicineHead;
    Medicine* temp;
    while (current != NULL) {
        fprintf(fp, "Full Name: %s, Abbreviation: %s, Stock: %d\n", current->fullName, current->abbreviation, current->stock);
        temp = current;
        current = current->next;
    }

    fclose(fp);
}

void importMedicine() {
    FILE* fp = fopen("storage\\MedicineList.txt", "r");
    Medicine* current;
    if (fp == NULL) {
        printf(Red("����")"�� storage\\MedicineList.txt ʧ�ܡ�\n");
        return;
    }
    if (medicineHead != NULL) {
        freeMedicine();
    }
    char fullName[100], abbreviation[20];
    int stock;
    while (medicineHead) {
        current = medicineHead->next;
        free(medicineHead);
        medicineHead = current;
    }
    while (!feof(fp)) {
        fscanf(fp, "Full Name: %[^,], Abbreviation: %[^,], Stock: %d\n", fullName, abbreviation, &stock);
        Medicine* newMedicine = (Medicine*)malloc(sizeof(Medicine));
        strcpy(newMedicine->fullName, fullName);
        strcpy(newMedicine->abbreviation, abbreviation);
        newMedicine->stock = stock;
        newMedicine->next = medicineHead;
        medicineHead = newMedicine;
    }
    fclose(fp);
}

Medicine* getMedicine(char abbr[ ]) {
    if (medicineHead == NULL) {
        importMedicine();
    }
    Medicine* current = medicineHead;
    while (current) {
        if (strcmp(current->abbreviation, abbr) == 0) {
            return current;
        }
    }
    return NULL;
}

void addMedicine() {
    Medicine* newMedicine = (Medicine*)malloc(sizeof(Medicine));
    newMedicine->stock = -1; // ��ʼ��
    displayInput("����ҩƷȫ��", "%[^\n]", newMedicine->fullName);
    displayInput("����ҩƷ��д", "%s", newMedicine->abbreviation);
    displayInput("����ҩƷ���", "%d", &newMedicine->stock);
    if (newMedicine->stock < 0) {
        printf(Red("����")"ҩƷ��治��Ϊ����\n");
    } else {
        newMedicine->next = medicineHead;
        medicineHead = newMedicine;
        exportMedicine();
        printf("�ɹ����ҩƷ��\n");
    }
}

void viewMedicine() {
    Medicine* current = medicineHead;
    if (current == NULL) {
        printf("ҩƷ��ϢΪ�ա�\n");
        return;
    }
    printf("��ǰҩƷ��Ϣ��\n");
    while (current != NULL) {
        printf("%s[%s] ���%d��λ\n", current->fullName, current->abbreviation, current->stock);
        current = current->next;
    }
}

void updateMedicine() {
    char abbr[20];
    displayInput("����ҩƷ��д", "%s", abbr);
    Medicine* medicine = getMedicine(abbr);
    if (medicine == NULL) {
        printf(Red("����")"δ�ҵ�ҩƷ %s��\n", abbr);
    }
    printf("%s ��ǰ��棺%d\n", medicine->fullName, medicine->stock);
    displayInput("�����¿����", "%d", &medicine->stock);
    exportMedicine();
    printf("�޸Ŀ�����ɹ���\n");
    return;
}

int ModifyStock(char abbr[ ], int quantity) {
    Medicine* medicine = getMedicine(abbr);
    if (medicine == NULL) {
        printf(Red("����")"δ�ҵ�ҩƷ %s��\n", abbr);
        return -2;
    }
    if (medicine->stock >= quantity) {
        medicine->stock -= quantity;
        exportMedicine();
        return 0;
    } else {
        printf(Red("����")"ҩƷ��治�㡣\n");
        return -1;
    }
}

void deleteMedicine() {
    char abbr[20];
    displayInput("����ҩƷ��д", "%s", abbr);
    Medicine* current = medicineHead, * prev = NULL;
    while (current != NULL) {
        if (strcmp(current->abbreviation, abbr) == 0) {
            if (prev == NULL) {
                medicineHead = current->next;
            } else {
                prev->next = current->next;
            }
            free(current);
            exportMedicine();
            printf("�ɹ�ɾ��ҩƷ��\n");
            return;
        }
        prev = current;
        current = current->next;
    }
    printf(Red("����")"δ�ҵ�ҩƷ %s��\n", abbr);
}

int medicineMain() {
    int choice;
    importMedicine();
    while (1) {
        choice = displaySelect("[����Ա] ҩƷ����", -5, "�����ҩƷ", "�鿴ҩƷ���", "����ҩƷ���", "ɾ��ҩƷ", "�˳�ҩƷ����");
        switch (choice) {
        case 0:
            addMedicine();
            break;
        case 1:
            viewMedicine();
            break;
        case 2:
            updateMedicine();
            break;
        case 3:
            deleteMedicine();
            break;
        case -1:
        case 4:
            freeMedicine();
            return 0;
        }
        system("pause > nul");
    }
}
