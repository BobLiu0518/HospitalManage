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
        printf(Red("错误：")"打开 storage\\MedicineList.txt 失败。\n");
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
        printf(Red("错误：")"打开 storage\\MedicineList.txt 失败。\n");
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
    newMedicine->stock = -1; // 初始化
    displayInput("输入药品全名", "%[^\n]", newMedicine->fullName);
    displayInput("输入药品缩写", "%s", newMedicine->abbreviation);
    displayInput("输入药品库存", "%d", &newMedicine->stock);
    if (newMedicine->stock < 0) {
        printf(Red("错误：")"药品库存不能为负。\n");
    } else {
        newMedicine->next = medicineHead;
        medicineHead = newMedicine;
        exportMedicine();
        printf("成功添加药品。\n");
    }
}

void viewMedicine() {
    Medicine* current = medicineHead;
    if (current == NULL) {
        printf("药品信息为空。\n");
        return;
    }
    printf("当前药品信息：\n");
    while (current != NULL) {
        printf("%s[%s] 库存%d单位\n", current->fullName, current->abbreviation, current->stock);
        current = current->next;
    }
}

void updateMedicine() {
    char abbr[20];
    displayInput("输入药品缩写", "%s", abbr);
    Medicine* medicine = getMedicine(abbr);
    if (medicine == NULL) {
        printf(Red("错误：")"未找到药品 %s。\n", abbr);
    }
    printf("%s 当前库存：%d\n", medicine->fullName, medicine->stock);
    displayInput("输入新库存数", "%d", &medicine->stock);
    exportMedicine();
    printf("修改库存数成功。\n");
    return;
}

int ModifyStock(char abbr[ ], int quantity) {
    Medicine* medicine = getMedicine(abbr);
    if (medicine == NULL) {
        printf(Red("错误：")"未找到药品 %s。\n", abbr);
        return -2;
    }
    if (medicine->stock >= quantity) {
        medicine->stock -= quantity;
        exportMedicine();
        return 0;
    } else {
        printf(Red("错误：")"药品库存不足。\n");
        return -1;
    }
}

void deleteMedicine() {
    char abbr[20];
    displayInput("输入药品缩写", "%s", abbr);
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
            printf("成功删除药品。\n");
            return;
        }
        prev = current;
        current = current->next;
    }
    printf(Red("错误：")"未找到药品 %s。\n", abbr);
}

int medicineMain() {
    int choice;
    importMedicine();
    while (1) {
        choice = displaySelect("[管理员] 药品管理", -5, "添加新药品", "查看药品库存", "更新药品库存", "删除药品", "退出药品管理");
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
