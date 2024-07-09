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
        printf(Red("错误：")"无法打开 storage\\user.txt。\n");
        return;
    }
    for (i = 0; i < 100; i++) {
        patient.user_type = 2;
        patient.id = 30000 + i;
        sprintf(patient.name, "Patient%03d", i + 1);
        sprintf(password, "PatientPass%d", i + 1);
        encryptPassword(password, patient.password);
        sprintf(patient.phone, "133%08d", rand() % 100000000);
        strcpy(patient.sex, (rand() % 2 == 0) ? "男" : "女");
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

    const char* departments[ ] = { "眼科", "耳鼻喉科", "肝胆胰外科", "甲乳外科", "创伤骨科", "心外科", "胸外科", "心内科", "神经内科", "血液科", "皮肤科" };
    const char* titles[ ] = { "主任医师", "副主任医师", "主治医师", "住院医师" };
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
        strcpy(doctor.sex, (rand() % 2 == 0) ? "男" : "女");
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
        strcpy(admin.sex, (rand() % 2 == 0) ? "男" : "女");
        admin.birth.year = 1980 + (rand() % 10);
        admin.birth.month = (rand() % 12) + 1;
        admin.birth.date = (rand() % 28) + 1;
        fprintf(fp, "0 %lld %s %s %s %s %d %d %d\n",
            admin.id, admin.name, admin.password, admin.phone, admin.sex,
            admin.birth.year, admin.birth.month, admin.birth.date);
    }

    encryptPassword("11111", password);
    fprintf(fp, "0 11111 Admin0 %s 18888888888 男 1906 9 6\n", password);
    encryptPassword("55555", password);
    fprintf(fp, "1 55555 袁张 病理科 院长 155555555555 女 %s 1999 9 9\n", password);
    encryptPassword("99999", password);
    fprintf(fp, "2 99999 王二麻子 %s 19999999999 男 2024 7 9", password);

    fclose(fp);
}

USERS* create_user() {
    USERS* new_user = (USERS*)malloc(sizeof(USERS));
    if (!new_user) {
        printf(Red("错误：")"内存分配失败。\n");
        exit(-1);
    }

    displayInput("输入用户类型（0管理 1医生 2患者）", "%d", &new_user->user_type);

    // 输入共同字段  
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
    if (new_user->user_type == 1) { // 医生  
        setbuf(stdin, NULL);
        printf("Enter department: ");
        fgets(new_user->department, sizeof(new_user->department), stdin);
        new_user->department[strcspn(new_user->department, "\n")] = 0;

        printf("Enter title: ");
        fgets(new_user->title, sizeof(new_user->title), stdin);
        new_user->title[strcspn(new_user->title, "\n")] = 0;
    }
    printf("加入成功");
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
        printf(Red("无法打开文件"));
        return -1;
    }
    int count = 0, user_type;
    USERS* new_user;
    while (fscanf(fp, "%d", &user_type) == 1) {
        new_user = (USERS*)malloc(sizeof(USERS));
        new_user->next = NULL;
        new_user->user_type = user_type;
        if (new_user == NULL) {
            printf(Red("内存分配失败\n"));
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
    displayInput("请输入账户名", "%lld", &input_id);
    displayInputPassword("请输入密码", input_password, 100);
    encryptPassword(input_password, encrypt_password);

    USERS* temp = user_head;
    while (temp != NULL) {
        if (temp->id == input_id) {
            if (strcmp(temp->password, encrypt_password) == 0) {
                printf("登录成功，欢迎 %s。\n", temp->name);
                return temp;
            } else {
                printf(Red("登录失败：")"密码错误。\n");
                return NULL;
            }
        }
        temp = temp->next;
    }

    printf(Red("登录失败：")"用户 %lld 不存在。\n", input_id);
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
        printf("未找到用户\n");
        return;
    }

    printf("用户ID: %lld\n", user->id);
    printf("用户名: %s\n", user->name);
    // printf("密码: %s\n", user->password);  // 实际应用中不打印密码  
    printf("电话: %s\n", user->phone);
    printf("性别: %s\n", user->sex);
    printf("生日: %d-%d-%d\n", user->birth.year, user->birth.month, user->birth.date);

    if (user->user_type == 1) {
        printf("部门: %s\n", user->department);
        printf("职称: %s\n", user->title);
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
    printf(Red("未找到用户ID为%lld的用户\n"), input_id);
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
            printf(Yellow("请输入要查找的用户ID: "));
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
            printf(Yellow("请输入要删除的用户ID: "));
            scanf("%lld", &id_to_delete);
            if (delete_user(id_to_delete) == 0) {
                printf(Yellow("用户删除成功\n"));
            }
            break;
        case 5:
            printf(Red("退出程序\n"));
            return 0;
        default:
            printf(Red("Invalid choice, please try again.\n"));
        }
    }
    return 0;
}