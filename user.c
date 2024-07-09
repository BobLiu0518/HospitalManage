#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "033.h"
#include "user.h"
#include "beautifulDisplay.h"
#include "passwordTools.h"

USERS* user_head = NULL;
void create_data() {
    int i;
    char password[100];
    USERS patient, doctor, admin;
    FILE* fp;
    srand((unsigned)time(NULL));
    fp = fopen("storage\\user.txt", "w");
    if (fp == NULL) {
        printf(Red("����")"�޷��� storage\\user.txt��\n");
        return;
    }
    for (i = 0; i < 100; i++) {
        patient.user_type = 2;
        patient.id = 30000 + i;
        sprintf(patient.name, "Patient%03d", i + 1);
        sprintf(password, "PatientPass%d", i + 1);
        encryptPassword(password, patient.password);
        sprintf(patient.phone, "133%08d", rand() % 100000000);
        strcpy(patient.sex, (rand() % 2 == 0) ? "��" : "Ů");
        patient.birth.year = 1930 + (rand() % 91);
        patient.birth.month = (rand() % 12) + 1;
        patient.birth.date = (rand() % 28) + 1;
        strcpy(patient.department, "");
        strcpy(patient.title, "");
        fprintf(fp, "2 %lld %s %s %s %s %d %d %d\n",
            patient.id, patient.name, patient.password,
            patient.phone, patient.sex,
            patient.birth.year, patient.birth.month, patient.birth.date);
    }

    const char* departments[ ] = { "�ۿ�", "���Ǻ��", "�ε������", "�������", "���˹ǿ�", "�����", "�����", "���ڿ�", "���ڿ�", "ѪҺ��", "Ƥ����" };
    const char* titles[ ] = { "����ҽʦ", "������ҽʦ", "����ҽʦ", "סԺҽʦ" };
    for (i = 0; i < 300; i++) {
        doctor.user_type = 1;
        doctor.id = 20000 + i;
        sprintf(doctor.name, "Doctor%d", i + 1);
        sprintf(password, "DoctorPass%d", i + 1);
        encryptPassword(password, doctor.password);
        sprintf(doctor.phone, "155%08d", rand() % 100000000);
        int dept_index = rand() % (sizeof(departments) / sizeof(departments[0]));
        int title_index = rand() % (sizeof(titles) / sizeof(titles[0]));
        strcpy(doctor.department, departments[dept_index]);
        strcpy(doctor.title, titles[title_index]);
        strcpy(doctor.sex, (rand() % 2 == 0) ? "��" : "Ů");
        doctor.birth.year = 1970 + (rand() % 30);
        doctor.birth.month = (rand() % 12) + 1;
        doctor.birth.date = (rand() % 28) + 1;
        fprintf(fp, "1 %lld %s %s %s %s %s %s %d %d %d\n",
            doctor.id, doctor.name, doctor.department, doctor.title,
            doctor.phone, doctor.sex, doctor.password,
            doctor.birth.year, doctor.birth.month, doctor.birth.date);
    }

    for (i = 0; i < 10; i++) {
        admin.user_type = 0;
        admin.id = 10000 + i;
        sprintf(admin.name, "Admin%d", i + 1);
        sprintf(password, "AdminPass%d", i + 1);
        encryptPassword(password, admin.password);
        sprintf(admin.phone, "188%08d", rand() % 100000000);
        strcpy(admin.department, "");
        strcpy(admin.title, "");
        strcpy(admin.sex, (rand() % 2 == 0) ? "��" : "Ů");
        admin.birth.year = 1980 + (rand() % 10);
        admin.birth.month = (rand() % 12) + 1;
        admin.birth.date = (rand() % 28) + 1;
        fprintf(fp, "0 %lld %s %s %s %s %d %d %d\n",
            admin.id, admin.name, admin.password, admin.phone, admin.sex,
            admin.birth.year, admin.birth.month, admin.birth.date);
    }

    encryptPassword("11111", password);
    fprintf(fp, "0 11111 Admin0 %s 18888888888 �� 1906 9 6\n", password);
    encryptPassword("55555", password);
    fprintf(fp, "1 55555 Ԭ�� ����� Ժ�� 155555555555 Ů %s 1999 9 9\n", password);
    encryptPassword("99999", password);
    fprintf(fp, "2 99999 �������� %s 19999999999 �� 2024 7 9", password);

    fclose(fp);
}

USERS* create_user() {
    USERS* new_user = (USERS*)malloc(sizeof(USERS));
    if (!new_user) {
        printf(Red("����")"�ڴ����ʧ�ܡ�\n");
        exit(-1);
    }

    displayInput("�����û����ͣ�0���� 1ҽ�� 2���ߣ�", "%d", &new_user->user_type);

    // ���빲ͬ�ֶ�  
    printf("Enter ID: ");
    scanf("%lld", &new_user->id);
    setbuf(stdin, NULL);
    printf("Enter name: ");
    fgets(new_user->name, sizeof(new_user->name), stdin);
    new_user->name[strcspn(new_user->name, "\n")] = 0;

    printf("Enter password: ");
    fgets(new_user->password, sizeof(new_user->password), stdin);
    new_user->password[strcspn(new_user->password, "\n")] = 0;

    printf("Enter phone: ");
    fgets(new_user->phone, sizeof(new_user->phone), stdin);
    new_user->phone[strcspn(new_user->phone, "\n")] = 0;

    printf("Enter sex: ");
    fgets(new_user->sex, sizeof(new_user->sex), stdin);
    new_user->sex[strcspn(new_user->sex, "\n")] = 0;
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
            fscanf(fp, "%lld %19s %99s %99s %99s %99s %32s %d %d %d",
                &new_user->id, new_user->name, new_user->department,
                new_user->title, new_user->phone, new_user->sex,
                new_user->password, &new_user->birth.year, &new_user->birth.month,
                &new_user->birth.date);
        } else {
            fscanf(fp, "%lld %19s %32s %99s %99s %d %d %d", &new_user->id,
                new_user->name, new_user->password, new_user->phone, new_user->sex,
                &new_user->birth.year, &new_user->birth.month, &new_user->birth.date);
        }
        add_user(new_user);
    }
}

USERS* login() {
    long long input_id;
    char input_password[100], encrypt_password[33];

    if (user_head == NULL) {
        read_user_data();
    }
    displayInput("�������˻���", "%lld", &input_id);
    displayInputPassword("����������", input_password, 100);
    encryptPassword(input_password, encrypt_password);

    USERS* temp = user_head;
    while (temp != NULL) {
        if (temp->id == input_id) {
            if (strcmp(temp->password, encrypt_password) == 0) {
                printf("��¼�ɹ�����ӭ %s��\n", temp->name);
                return temp;
            } else {
                printf(Red("��¼ʧ�ܣ�")"�������\n");
                return NULL;
            }
        }
        temp = temp->next;
    }

    printf(Red("��¼ʧ�ܣ�")"�û� %lld �����ڡ�\n", input_id);
    return NULL;
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
    // create_data();
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
            USERS* user;
            printf(Yellow("������Ҫ���ҵ��û�ID: "));
            scanf("%lld", &id_to_find);
            user = find_user_by_id(id_to_find);
            if (user != NULL) {
                display_user_info(user);
            }
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
            printf(Red("�˳�����\n"));
            return 0;
        default:
            printf(Red("Invalid choice, please try again.\n"));
        }
    }
    return 0;
}