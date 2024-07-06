#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Medicine {
    char fullName[100];
    char abbreviation[20];
    int stock;
    struct Medicine *next;
} Medicine;

Medicine *head = NULL;

// ¼��ҩƷ
void addMedicine() {
    Medicine *newMedicine = (Medicine *)malloc(sizeof(Medicine));
    newMedicine->stock=-1;//��ʼ��
    printf("Enter full name of medicine: ");
    scanf("%s", newMedicine->fullName);
    printf("Enter abbreviation of medicine: ");
    scanf("%s", newMedicine->abbreviation);
    printf("Enter stock quantity: ");
    scanf("%d", &newMedicine->stock);
      if(newMedicine->stock<0)
      printf("Invalid choice. Please try again.\n");
      else
      {
    newMedicine->next = head;
    head = newMedicine;
    printf("Medicine added successfully!\n");
    }
}
// �鿴����ҩƷ
void viewMedicine() {
    Medicine *current = head;
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
    Medicine *current = head;
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
void reduceMedicine() {
    char abbr[20];
    int quantity;
    printf("Enter abbreviation of medicine: ");
    scanf("%s", abbr);
    printf("Enter quantity to reduce: ");
    scanf("%d", &quantity);
    Medicine *current = head;
    while (current != NULL) {
        if (strcmp(current->abbreviation, abbr) == 0) {
            if (current->stock >= quantity) {
                current->stock -= quantity;
                printf("Stock reduced successfully!\n");
                return;
            } else {
                printf("Not enough stock!\n");
                return;
            }
        }
        current = current->next;
    }
    printf("Medicine not found!\n");
}

int main() {
    char choice;
    while (1) {
        printf("Welcome to Medicine Management System!\n");
        printf("\n1. Add Medicine\n2. View Medicine\n3. Update Medicine Stock\n4. Reduce Medicine Stock\n5. Exit\nEnter your choice: ");
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
                reduceMedicine();
                break;
            case '5':
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
                setbuf(stdin, NULL);
    }
    return 0;
}
