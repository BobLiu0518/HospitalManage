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

int read_user_data() {
    FILE* fp = fopen("storage\\user.txt", "r");
    if (fp == NULL) {
        printf(Red("错误：")"无法打开文件。\n");
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
            printf(Red("错误：")"内存分配失败。\n");
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
        printf(Red("错误：")"无法打开文件。\n");
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
        printf(Red("错误：")"内存分配失败。\n");
        exit(-1);
    }

    if (is_admin) {
        new_user->user_type = displaySelect("选择新用户类型", -3, "管理员", "医生", "患者");
        if (new_user->user_type == -1) {
            return -1;
        }
    } else {
        new_user->user_type = 2;
    }

    displayTitle(is_admin ? "添加新用户" : "注册账号");
    if (new_user->user_type == 2) {
        displayInput("输入就诊卡号", "%lld", &new_user->id);
    } else {
        displayInput("输入工号", "%lld", &new_user->id);
    }

    if (new_user->id <= 0) {
        printf(Red("错误：")"就诊卡号或工号需为正整数。\n");
        return -1;
    } else if (new_user->id < 10000) {
        printf(Red("错误：")"就诊卡号或工号不能小于5位。\n");
        return -1;
    }

    temp = user_head;
    while (temp) {
        if (temp->id == new_user->id) {
            printf(Red("错误：")"用户已存在，请直接登录。\n");
            return -1;
        }
        temp = temp->next;
    }

    displayInput("输入姓名", "%[^\n]", new_user->name);
    displayInputPassword("输入密码", password, 100);
    encryptPassword(password, new_user->password);
    if (strlen(new_user->password) < 8) {
        printf(Red("错误：")"密码不能小于8位。\n");
        return -1;
    }
    displayInput("输入手机号", "%s", new_user->phone);
    if (strlen(new_user->phone) != 11) {
        printf(Red("错误：")"手机号需为中国大陆11位手机号。\n");
        return -1;
    }
    sex = displaySelect("选择性别", -2, "男", "女");
    if (sex == -1) {
        return -1;
    }
    strcpy(new_user->sex, sex == 0 ? "男" : "女");
    displayTitle(is_admin ? "添加新用户" : "注册账号");
    displayInput("输入出生年份", "%d", &new_user->birth.year);
    displayInput("输入出生月份", "%d", &new_user->birth.month);
    displayInput("输入出生日期", "%d", &new_user->birth.date);
    int dates[ ] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
    Datetime now = getDateTime();
    if (new_user->birth.year < 1850 || new_user->birth.month < 1 || new_user->birth.date < 1 ||
        new_user->birth.month > 12 || new_user->birth.date > dates[new_user->birth.month - 1]) {
        printf(Red("错误：")"出生日期不合法。\n");
        return -1;
    }
    if (new_user->birth.year > now.year || (new_user->birth.year == now.year && new_user->birth.month > now.month) ||
        (new_user->birth.year == now.year && new_user->birth.month == now.month && new_user->birth.date > now.day)) {
        printf(Red("错误：")"出生日期不得早于当前日期。\n");
        return -1;
    }

    if (new_user->user_type == 1) {
        displayInput("输入部门", "%[^\n]", new_user->department);
        char* titles[ ] = { "主任医师", "副主任医师", "主治医师", "住院医师" };
        strcpy(new_user->title, titles[displaySelect("选择职称", 4, titles)]);
    }
    new_user->next = NULL;

    displayTitle(is_admin ? "添加新用户" : "注册账号");
    if (is_admin) {
        printf("添加新用户成功。\n");
    } else {
        printf("注册成功。\n");
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
    displayTitle("登录");
    displayInput("请输入就诊卡号或工号", "%lld", &input_id);

    USERS* user = find_user_by_id(input_id);
    if (user == NULL) {
        printf(Red("登录失败：")"用户 %lld 不存在。\n", input_id);
        system("pause > nul");
        return NULL;
    }
    displayInputPassword("请输入密码", input_password, 100);
    encryptPassword(input_password, encrypt_password);
    if (strcmp(user->password, encrypt_password) == 0) {
        printf("登录成功，欢迎 %s。\n", user->name);
        system("pause > nul");
        return user;
    } else {
        printf(Red("登录失败：")"密码错误。\n");
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
    displayTitle("用户信息");
    if (user == NULL) {
        printf("未找到用户\n");
        return;
    }

    printf("用户ID: %lld\n", user->id);
    printf("用户名: %s\n", user->name);
    // printf("密码:   %s\n", user->password);  // 实际应用中不打印密码  
    printf("电话:   %s\n", user->phone);
    printf("性别:   %s\n", user->sex);
    printf("生日:   %d/%d/%d\n", user->birth.year, user->birth.month, user->birth.date);

    if (user->user_type == 1) {
        printf("部门:   %s\n", user->department);
        printf("职称:   %s\n", user->title);
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
        choice = displaySelect("[管理员] 用户管理", -4, "添加用户", "查找用户", "删除用户", "退出用户管理");

        switch (choice) {
        case 0:
            create_user(1);
            break;
        case 1:
            long long id_to_find;
            USERS* user;
            displayTitle("查找用户");
            displayInput("输入要查找的用户ID", "%lld", &id_to_find);
            user = find_user_by_id(id_to_find);
            if (user != NULL) {
                display_user_info(user);
            } else {
                printf("未找到用户ID为 %lld 的用户。\n", id_to_find);
            }
            break;
        case 2:
            long long id_to_delete;
            displayTitle("删除用户");
            displayInput("输入要删除的用户ID", "%lld", &id_to_delete);
            if (id_to_delete == current->id) {
                printf("无法删除当前登录的用户。\n");
            } else if (!displaySelect("确认要删除吗？", -2, "是", "否")) {
                if (delete_user(id_to_delete) == 0) {
                    printf("删除用户 %lld 成功。\n", id_to_delete);
                } else {
                    printf("未找到用户ID为 %lld 的用户。\n", id_to_delete);
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