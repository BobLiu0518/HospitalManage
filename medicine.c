#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "medicine.h"

Medicine* head = NULL;

// ¼��ҩƷ
void addMedicine() {
    Medicine* newMedicine = (Medicine*)malloc(sizeof(Medicine));
    newMedicine->stock = -1;//��ʼ��
    printf("Enter full name of medicine: ");
    scanf("%s", newMedicine->fullName);
    printf("Enter abbreviation of medicine: ");
    scanf("%s", newMedicine->abbreviation);
    printf("Enter stock quantity: ");
    scanf("%d", &newMedicine->stock);
    if (newMedicine->stock < 0)
        printf("Invalid choice. Please try again.\n");
    else {
        newMedicine->next = head;
        head = newMedicine;
        printf("Medicine added successfully!\n");
    }
}
// �鿴����ҩƷ
void viewMedicine() {
    Medicine* current = head;
    if (current == NULL) {
        printf("No medicines recorded.\n");
        return;
    }
    printf("List of Medicines:\n");
    while (current != NULL) {
        printf("Full Name: %s, Abbreviation: %s, Stock: %d\n", current->fullName, current->abbreviation, current->stock);
        current = current->next;
    }
}

// �޸�ҩƷ���
void updateMedicine() {
    char abbr[20];
    printf("Enter abbreviation of medicine to update: ");
    scanf("%s", abbr);
    Medicine* current = head;
    while (current != NULL) {
        if (strcmp(current->abbreviation, abbr) == 0) {
            printf("Enter new stock quantity: ");
            scanf("%d", &current->stock);
            printf("Stock updated successfully!\n");
            return;
        }
        current = current->next;
    }
    printf("Medicine not found!\n");
}

// ����ҩƷ���
int ModifyStock(char abbr[ ], int quantity) {
    Medicine* current = head;
    while (current != NULL) {
        if (strcmp(current->abbreviation, abbr) == 0) {
            if (current->stock >= quantity) {
                current->stock += quantity;
                printf("Stock modified successfully!\n");
                return 0;
            } else {
                printf("Not enough stock!\n");
                return -1;
            }
        }
        current = current->next;
    }
    printf("Medicine not found!\n");
    return -2;
}
void deleteMedicine() {
    char abbr[20];
    printf("Enter abbreviation of medicine to delete: ");
    scanf("%s", abbr);
    Medicine* current = head, * prev = NULL;
    while (current != NULL) {
        if (strcmp(current->abbreviation, abbr) == 0) {
            if (prev == NULL) {
                head = current->next;
            } else {
                prev->next = current->next;
            }
            free(current);
            printf("Medicine deleted successfully!\n");
            return;
        }
        prev = current;
        current = current->next;
    }
    printf("Medicine not found!\n");
}


void exportMedicine() {
    FILE* fp = fopen("storage\\MedicineList.txt", "w");
    if (fp == NULL) {
        printf("Failed to open file.\n");
        return;
    }

    // ����һ����ʱָ������������
    Medicine* current = head;
    Medicine* temp;

    // ��������
    while (current != NULL) {
        fprintf(fp, "Full Name: %s, Abbreviation: %s, Stock: %d\n", current->fullName, current->abbreviation, current->stock);
        temp = current; // ���浱ǰ�ڵ�
        current = current->next; // �ƶ�����һ���ڵ�
    }

    fclose(fp);
    printf("Medicine list exported and cleared successfully!\n");
}

// ����ҩƷ�嵥
void importMedicine() {
    FILE* fp = fopen("storage\\MedicineList.txt", "r");
    Medicine* current;
    if (fp == NULL) {
        printf("Failed to open file.\n");
        return;
    }
    char fullName[100], abbreviation[20];
    int stock;
    while (head) {
        current = head->next;
        free(head);
        head = current;
    }
    while (!feof(fp)) {
        fscanf(fp, "Full Name: %[^,], Abbreviation: %[^,], Stock: %d\n", fullName, abbreviation, &stock);
        Medicine* newMedicine = (Medicine*)malloc(sizeof(Medicine));
        strcpy(newMedicine->fullName, fullName);
        strcpy(newMedicine->abbreviation, abbreviation);
        newMedicine->stock = stock;
        newMedicine->next = head;
        head = newMedicine;
    }
    fclose(fp);
    printf("Medicine list imported successfully!\n");
}

int medicineMain() {
    char choice;
    while (1) {
        printf("Welcome to Medicine Management System!\n");
        printf("\n1. Add Medicine\n2. View Medicine\n3. Update Medicine Stock(admin)\n4. Delete Medicine\n5. Export Medicine List\n6. Import Medicine List\n7. Exit\nEnter your choice: ");
        scanf("%c", &choice);
        switch (choice) {
        case '1':
            addMedicine();
            break;
        case '2':
            viewMedicine();
            break;
        case '3':
            updateMedicine();
            break;
        case '4':
            deleteMedicine();
            break;
        case '5':
            exportMedicine();
            break;
        case '6':
            importMedicine();
            break;
        case '7':
            exit(0);
        default:
            printf("Invalid choice. Please try again.\n");
        }
        setbuf(stdin, NULL);
    }
    return 0;
}
