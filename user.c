#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "033.h"
#include "user.h"
#include "beautifulDisplay.h"
#include "timeTools.h"
#include "passwordTools.h"

USERS* user_head = NULL;
void create_data() {
    int i;
    char password[100];
    USERS patient, doctor, admin;
    FILE* fp;
    srand((unsigned)time(NULL));
    free_users();
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

int read_user_data() {
    FILE* fp = fopen("storage\\user.txt", "r");
    if (fp == NULL) {
        printf(Red("����")"�޷����ļ���\n");
        return -1;
    }
    int count = 0, user_type;
    USERS* new_user;
    if (user_head != NULL) {
        free_users();
    }
    while (fscanf(fp, "%d", &user_type) == 1) {
        new_user = (USERS*)malloc(sizeof(USERS));
        if (new_user == NULL) {
            printf(Red("����")"�ڴ����ʧ�ܡ�\n");
            fclose(fp);
            return -1;
        }

        new_user->next = NULL;
        new_user->user_type = user_type;
        if (new_user->user_type == 1) {
            fscanf(fp, "%lld %19s %99s %99s %99s %99s %32s %d %d %d\n",
                &new_user->id, new_user->name, new_user->department,
                new_user->title, new_user->phone, new_user->sex,
                new_user->password, &new_user->birth.year, &new_user->birth.month,
                &new_user->birth.date);
        } else {
            fscanf(fp, "%lld %19s %32s %99s %99s %d %d %d\n", &new_user->id,
                new_user->name, new_user->password, new_user->phone, new_user->sex,
                &new_user->birth.year, &new_user->birth.month, &new_user->birth.date);
        }
        add_user(new_user);
    }
    fclose(fp);
    return 0;
}

int save_user_data() {
    FILE* fp = fopen("storage\\user.txt", "w");
    if (fp == NULL) {
        printf(Red("����")"�޷����ļ���\n");
        return -1;
    }
    int i;
    USERS* temp = user_head;
    while (temp) {
        if (temp->user_type == 1) {
            fprintf(fp, "%d %lld %s %s %s %s %s %s %d %d %d\n", temp->user_type,
                temp->id, temp->name, temp->department,
                temp->title, temp->phone, temp->sex,
                temp->password, temp->birth.year, temp->birth.month,
                temp->birth.date);
        } else {
            fprintf(fp, "%d %lld %s %s %s %s %d %d %d\n", temp->user_type, temp->id,
                temp->name, temp->password, temp->phone, temp->sex,
                temp->birth.year, temp->birth.month, temp->birth.date);
        }
        temp = temp->next;
    }
    fclose(fp);
    return 0;
}

int create_user(int is_admin) {
    USERS* new_user = malloc(sizeof(USERS)), * temp;
    int sex;
    char password[100];

    if (!new_user) {
        printf(Red("����")"�ڴ����ʧ�ܡ�\n");
        exit(-1);
    }

    if (is_admin) {
        new_user->user_type = displaySelect("ѡ�����û�����", -3, "����Ա", "ҽ��", "����");
        if (new_user->user_type == -1) {
            return -1;
        }
    } else {
        new_user->user_type = 2;
    }

    displayTitle(is_admin ? "������û�" : "ע���˺�");
    if (new_user->user_type == 2) {
        displayInput("������￨��", "%lld", &new_user->id);
    } else {
        displayInput("���빤��", "%lld", &new_user->id);
    }

    if (new_user->id <= 0) {
        printf(Red("����")"���￨�Ż򹤺���Ϊ��������\n");
        return -1;
    } else if (new_user->id < 10000) {
        printf(Red("����")"���￨�Ż򹤺Ų���С��5λ��\n");
        return -1;
    }

    temp = user_head;
    while (temp) {
        if (temp->id == new_user->id) {
            printf(Red("����")"�û��Ѵ��ڣ���ֱ�ӵ�¼��\n");
            return -1;
        }
        temp = temp->next;
    }

    displayInput("��������", "%[^\n]", new_user->name);
    displayInputPassword("��������", password, 100);
    encryptPassword(password, new_user->password);
    if (strlen(new_user->password) < 8) {
        printf(Red("����")"���벻��С��8λ��\n");
        return -1;
    }
    displayInput("�����ֻ���", "%s", new_user->phone);
    if (strlen(new_user->phone) != 11) {
        printf(Red("����")"�ֻ�����Ϊ�й���½11λ�ֻ��š�\n");
        return -1;
    }
    sex = displaySelect("ѡ���Ա�", -2, "��", "Ů");
    if (sex == -1) {
        return -1;
    }
    strcpy(new_user->sex, sex == 0 ? "��" : "Ů");
    displayTitle(is_admin ? "������û�" : "ע���˺�");
    displayInput("����������", "%d", &new_user->birth.year);
    displayInput("��������·�", "%d", &new_user->birth.month);
    displayInput("�����������", "%d", &new_user->birth.date);
    int dates[ ] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
    Datetime now = getDateTime();
    if (new_user->birth.year < 1850 || new_user->birth.month < 1 || new_user->birth.date < 1 ||
        new_user->birth.month > 12 || new_user->birth.date > dates[new_user->birth.month - 1]) {
        printf(Red("����")"�������ڲ��Ϸ���\n");
        return -1;
    }
    if (new_user->birth.year > now.year || (new_user->birth.year == now.year && new_user->birth.month > now.month) ||
        (new_user->birth.year == now.year && new_user->birth.month == now.month && new_user->birth.date > now.day)) {
        printf(Red("����")"�������ڲ������ڵ�ǰ���ڡ�\n");
        return -1;
    }

    if (new_user->user_type == 1) {
        displayInput("���벿��", "%[^\n]", new_user->department);
        char* titles[ ] = { "����ҽʦ", "������ҽʦ", "����ҽʦ", "סԺҽʦ" };
        strcpy(new_user->title, titles[displaySelect("ѡ��ְ��", 4, titles)]);
    }
    new_user->next = NULL;

    displayTitle(is_admin ? "������û�" : "ע���˺�");
    if (is_admin) {
        printf("������û��ɹ���\n");
    } else {
        printf("ע��ɹ���\n");
    }

    add_user(new_user);
    save_user_data();
    system("pause > nul");
    return 0;
}

void add_user(USERS* new_user) {
    if (new_user == NULL) {
        return;
    }
    if (user_head == NULL) {
        user_head = new_user;
        new_user->next = NULL;
    } else {
        new_user->next = user_head;
        user_head = new_user;
    }
}

USERS* login() {
    long long input_id;
    char input_password[100], encrypt_password[33];

    if (user_head == NULL) {
        read_user_data();
    }
    displayTitle("��¼");
    displayInput("��������￨�Ż򹤺�", "%lld", &input_id);

    USERS* user = find_user_by_id(input_id);
    if (user == NULL) {
        printf(Red("��¼ʧ�ܣ�")"�û� %lld �����ڡ�\n", input_id);
        system("pause > nul");
        return NULL;
    }
    displayInputPassword("����������", input_password, 100);
    encryptPassword(input_password, encrypt_password);
    if (strcmp(user->password, encrypt_password) == 0) {
        printf("��¼�ɹ�����ӭ %s��\n", user->name);
        system("pause > nul");
        return user;
    } else {
        printf(Red("��¼ʧ�ܣ�")"�������\n");
        system("pause > nul");
        return NULL;
    }

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
    displayTitle("�û���Ϣ");
    if (user == NULL) {
        printf("δ�ҵ��û�\n");
        return;
    }

    printf("�û�ID: %lld\n", user->id);
    printf("�û���: %s\n", user->name);
    // printf("����:   %s\n", user->password);  // ʵ��Ӧ���в���ӡ����  
    printf("�绰:   %s\n", user->phone);
    printf("�Ա�:   %s\n", user->sex);
    printf("����:   %d/%d/%d\n", user->birth.year, user->birth.month, user->birth.date);

    if (user->user_type == 1) {
        printf("����:   %s\n", user->department);
        printf("ְ��:   %s\n", user->title);
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
            save_user_data();
            return 0;
        }
        prev = temp;
        temp = temp->next;
    }
    return -1;
}

int user_main(USERS* current) {
    int choice;
    USERS* new_user = NULL;
    while (1) {
        choice = displaySelect("[����Ա] �û�����", -4, "����û�", "�����û�", "ɾ���û�", "�˳��û�����");

        switch (choice) {
        case 0:
            create_user(1);
            break;
        case 1:
            long long id_to_find;
            USERS* user;
            displayTitle("�����û�");
            displayInput("����Ҫ���ҵ��û�ID", "%lld", &id_to_find);
            user = find_user_by_id(id_to_find);
            if (user != NULL) {
                display_user_info(user);
            } else {
                printf("δ�ҵ��û�IDΪ %lld ���û���\n", id_to_find);
            }
            break;
        case 2:
            long long id_to_delete;
            displayTitle("ɾ���û�");
            displayInput("����Ҫɾ�����û�ID", "%lld", &id_to_delete);
            if (id_to_delete == current->id) {
                printf("�޷�ɾ����ǰ��¼���û���\n");
            } else if (!displaySelect("ȷ��Ҫɾ����", -2, "��", "��")) {
                if (delete_user(id_to_delete) == 0) {
                    printf("ɾ���û� %lld �ɹ���\n", id_to_delete);
                } else {
                    printf("δ�ҵ��û�IDΪ %lld ���û���\n", id_to_delete);
                }
            } else {
                continue;
            }
            break;
        case -1:
        case 3:
            return 0;
        }
        system("pause > nul");
    }
}