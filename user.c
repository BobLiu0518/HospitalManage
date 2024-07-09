#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include "033.h"
#include "user.h"

USERS* user;
// ����ͷָ��  
USERS* user_head = NULL;
void create_data() {
    int i, j, k;
    USERS patients[50];
    FILE* fp;
    fp = fopen("storage\\user.txt", "w");
    if (fp == NULL) {
        printf("Error opening file");
        return;
    }
    for (int i = 0; i < 50; i++) {
        patients[i].user_type = 2; // ����  
        patients[i].id = 30000 + i + 1; // ����ID��30001��ʼ  
        snprintf(patients[i].name, sizeof(patients[i].name), "Patient%03d", i + 1);
        snprintf(patients[i].password, sizeof(patients[i].password), "PatientPass%d", i + 1);
        snprintf(patients[i].phone, sizeof(patients[i].phone), "138%09d", (i * 123456789) % 1000000000);
        strcpy(patients[i].sex, (i % 2 == 0) ? "��" : "Ů");
        patients[i].birth.year = 1930 + (i % 91);
        patients[i].birth.month = (i % 12) + 1;
        patients[i].birth.date = (i % 28) + 1;
        strcpy(patients[i].department, "");
        strcpy(patients[i].title, "");
    }

    for (i = 0; i < 50; i++) {
        fprintf(fp, "2 %lld %s %s %s %s %d %d %d\n",
            patients[i].id, patients[i].name, patients[i].password,
            patients[i].phone, patients[i].sex,
            patients[i].birth.year, patients[i].birth.month, patients[i].birth.date);
    }   printf("%d\n", i);
    USERS doctors[30]; // �������Ѿ�����һ���㹻����������洢ҽ����Ϣ  

    for (int j = 0; j < 30; j++) {
        doctors[j].user_type = 1; // ҽ��  
        doctors[j].id = 20000 + j + 1; // ����ID��20001��ʼ  

        snprintf(doctors[j].name, sizeof(doctors[j].name), "Doctor%d", j + 1);
        snprintf(doctors[j].password, sizeof(doctors[j].password), "DoctorPass%d", j + 1);
        snprintf(doctors[j].phone, sizeof(doctors[j].phone), "123456789%d", j + 1);

        const char* departments[ ] = { "�ڿ�", "���", "����", "������", "�ǿ�", "�ۿ�" };
        const char* titles[ ] = { "����ҽʦ", "������ҽʦ", "����ҽʦ", "סԺҽʦ" };
        int dept_index = j % (sizeof(departments) / sizeof(departments[0]));
        int title_index = (j / 5) % (sizeof(titles) / sizeof(titles[0])); // ����ÿ5��ҽ������һ����ͬ��ְ��  
        strcpy(doctors[j].department, departments[dept_index]);
        strcpy(doctors[j].title, titles[title_index]);
        strcpy(doctors[j].sex, (j % 2 == 0) ? "��" : "Ů");

        doctors[j].birth.year = 1970 + (j % 30); // ����ҽ������ֲ���40-70��֮��  
        doctors[j].birth.month = (j % 12) + 1;
        doctors[j].birth.date = (j % 28) + 1; // �򻯴����������·���������  
    }

    for (j = 0; j < 30; j++) {
        fprintf(fp, "1 %lld %s %s %s %s %s %d %d %d\n",
            doctors[j].id, doctors[j].name, doctors[j].department, doctors[j].title,
            doctors[j].phone, doctors[j].sex, doctors[j].birth.year, doctors[j].birth.month, doctors[j].birth.date);
    }
    printf("%d\n", j);
    USERS admins[10];
    for (int k = 0; k < 10; k++) {
        admins[k].user_type = 0;
        admins[k].id = 10000 + k + 1; // ����ID��10001��ʼ    
        snprintf(admins[k].name, sizeof(admins[k].name), "Admin%d", k + 1);
        snprintf(admins[k].password, sizeof(admins[k].password), "AdminPass%d", k + 1);
        snprintf(admins[k].phone, sizeof(admins[k].phone), "12345%05d", k + 1);
        strcpy(admins[k].department, "");
        strcpy(admins[k].title, "");
        strcpy(admins[k].sex, (k % 2 == 0) ? "��" : "Ů");
        admins[k].birth.year = 1980 + (k % 10);
        admins[k].birth.month = (k % 12) + 1;
        admins[k].birth.date = (k % 28) + 1; // �򻯴����������·���������    
    }

    for (k = 0; k < 10; k++) {
        fprintf(fp, "0  %lld %s %s %s %d %d %d\n",
            admins[k].id, admins[k].name, admins[k].phone, admins[k].sex,
            admins[k].birth.year, admins[k].birth.month, admins[k].birth.date);

    }     printf("%d\n", k);
    fclose(fp);
}
USERS* create_user() {
    USERS* new_user = (USERS*)malloc(sizeof(USERS));
    if (!new_user) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    printf("Enter user type (0: Admin, 1: Doctor, 2: Patient): ");
    scanf("%d", &new_user->user_type);

    // ���빲ͬ�ֶ�  
    printf("Enter ID: ");
    scanf("%lld", &new_user->id);
    setbuf(stdin, NULL);
    printf("Enter name: ");
    fgets(new_user->name, sizeof(new_user->name), stdin);
    new_user->name[strcspn(new_user->name, "\n")] = 0; // �Ƴ�fgets��ȡ�Ļ��з�  

    printf("Enter password: ");
    fgets(new_user->password, sizeof(new_user->password), stdin);
    new_user->password[strcspn(new_user->password, "\n")] = 0; // �Ƴ�fgets��ȡ�Ļ��з�  

    printf("Enter phone: ");
    fgets(new_user->phone, sizeof(new_user->phone), stdin);
    new_user->phone[strcspn(new_user->phone, "\n")] = 0; // �Ƴ�fgets��ȡ�Ļ��з�  

    printf("Enter sex: ");
    fgets(new_user->sex, sizeof(new_user->sex), stdin);
    new_user->sex[strcspn(new_user->sex, "\n")] = 0; // �Ƴ�fgets��ȡ�Ļ��з�  
    printf("Enter birth year: ");
    scanf("%d", &new_user->birth.year);
    printf("Enter birth month: ");
    scanf("%d", &new_user->birth.month);
    printf("Enter birth date: ");
    scanf("%d", &new_user->birth.date);
    if (new_user->user_type == 1) { // ҽ��  
        setbuf(stdin, NULL);
        printf("Enter department: ");
        fgets(new_user->department, sizeof(new_user->department), stdin);
        new_user->department[strcspn(new_user->department, "\n")] = 0;

        printf("Enter title: ");
        fgets(new_user->title, sizeof(new_user->title), stdin);
        new_user->title[strcspn(new_user->title, "\n")] = 0;
    }
    printf("����ɹ�");
    new_user->next = NULL;

    return new_user;
}
void add_user(USERS* new_user) {
    if (user_head == NULL) {
        user_head = new_user;
        new_user->next = NULL;
    } else {
        /*        USERS* temp = user_head;
                while (temp->next != NULL) {
                    temp = temp->next;
                }
                temp->next = new_user;  */
        new_user->next = user_head;
        user_head = new_user;
    }
}
int read_user_data() {
    FILE* fp = fopen("storage\\user.txt", "r");
    if (fp == NULL) {
        printf(Red("�޷����ļ�"));
        return -1;
    }
    int count = 0, user_type;
    USERS* new_user;
    while (fscanf(fp, "%d", &user_type) == 1) {
        new_user = (USERS*)malloc(sizeof(USERS));
        new_user->next = NULL;
        new_user->user_type = user_type;
        if (new_user == NULL) {
            printf(Red("�ڴ����ʧ��\n"));
            return 0;
        }

        if (new_user->user_type == 1) {
            fscanf(fp, "%lld %19s %99s %99s %99s %99s %19s %d %d %d",
                &new_user->id, new_user->name, new_user->department,
                new_user->title, new_user->phone, new_user->sex
                , new_user->password, &new_user->birth.year, &new_user->birth.month,
                &new_user->birth.date);

        } else {
            fscanf(fp, "%lld %19s %99s %99s %99s %d %d %d", &new_user->id, new_user->name, new_user->password,
                new_user->phone, new_user->sex, &new_user->birth.year,
                &new_user->birth.month, &new_user->birth.date);
        }
        add_user(new_user);
    }
}

int login() {
    long long input_id;
    char input_password[100];

    printf("�������û�ID: ");
    scanf("%lld", &input_id);
    printf("����������: ");
    scanf("%99s", input_password);

    USERS* temp = user_head;
    while (temp != NULL) {
        if (temp->id == input_id && strcmp(temp->password, input_password) == 0) {
            printf("��¼�ɹ�!\n");
            return 1;
        }
        temp = temp->next;
    }

    printf("�û������������!\n");
    return 0;
}


void free_users() {
    USERS* temp;
    while (user_head != NULL) {
        temp = user_head;
        user_head = user_head->next;
        free(temp);
    }
}
USERS* find_user_by_id(long long input_id) {
    USERS* temp = user_head;
    while (temp != NULL) {
        if (temp->id == input_id) {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

void display_user_info(USERS* user) {
    if (user == NULL) {
        printf("δ�ҵ��û�\n");
        return;
    }

    printf("�û�ID: %lld\n", user->id);
    printf("�û���: %s\n", user->name);
    // printf("����: %s\n", user->password);  // ʵ��Ӧ���в���ӡ����  
    printf("�绰: %s\n", user->phone);
    printf("�Ա�: %s\n", user->sex);
    printf("����: %d-%d-%d\n", user->birth.year, user->birth.month, user->birth.date);

    if (user->user_type == 1) {
        printf("����: %s\n", user->department);
        printf("ְ��: %s\n", user->title);
    }
}

int delete_user(long long input_id) {
    USERS* temp = user_head, * prev = NULL;
    while (temp != NULL) {
        if (temp->id == input_id) {
            if (prev == NULL) {
                user_head = temp->next;
            } else {
                prev->next = temp->next;
            }
            free(temp);
            return 0;
        }
        prev = temp;
        temp = temp->next;
    }
    printf(Red("δ�ҵ��û�IDΪ%lld���û�\n"), input_id);
    return -1;
}

int user_main() {
    user_head = NULL;
    create_data();
    read_user_data();
    int choice;
    USERS* new_user = NULL;
    while (1) {
        printf("\nChoose an option:\n");
        printf("1. Add person\n");
        printf("2. Find person\n");
        printf("3. login\n");
        printf("4. Delete person\n");
        printf("5. Exit\n");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            new_user = create_user();
            add_user(new_user);
            break;
        case 2:

            long long id_to_find;
            printf(Yellow("������Ҫ���ҵ��û�ID: "));
            scanf("%lld", &id_to_find);

            user = find_user_by_id(id_to_find);
            if (user != NULL) {
                display_user_info(user);
            }
            free_users();
            break;
        case 3:
            login();
            break;
        case 4:
            long long id_to_delete;
            printf(Yellow("������Ҫɾ�����û�ID: "));
            scanf("%lld", &id_to_delete);
            if (delete_user(id_to_delete) == 0) {
                printf(Yellow("�û�ɾ���ɹ�\n"));
            }
            break;
        case 5:
            printf(Red("�˳����� \n"));
            return 0;
        default:
            printf(Red("Invalid choice, please try again.\n"));
        }
    }
    return 0;
}