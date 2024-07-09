#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include "033.h"
#include "user.h"

USERS* user;
// 链表头指针  
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
        patients[i].user_type = 2; // 患者  
        patients[i].id = 30000 + i + 1; // 假设ID从30001开始  
        snprintf(patients[i].name, sizeof(patients[i].name), "Patient%03d", i + 1);
        snprintf(patients[i].password, sizeof(patients[i].password), "PatientPass%d", i + 1);
        snprintf(patients[i].phone, sizeof(patients[i].phone), "138%09d", (i * 123456789) % 1000000000);
        strcpy(patients[i].sex, (i % 2 == 0) ? "男" : "女");
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
    USERS doctors[30]; // 假设你已经有了一个足够大的数组来存储医生信息  

    for (int j = 0; j < 30; j++) {
        doctors[j].user_type = 1; // 医生  
        doctors[j].id = 20000 + j + 1; // 假设ID从20001开始  

        snprintf(doctors[j].name, sizeof(doctors[j].name), "Doctor%d", j + 1);
        snprintf(doctors[j].password, sizeof(doctors[j].password), "DoctorPass%d", j + 1);
        snprintf(doctors[j].phone, sizeof(doctors[j].phone), "123456789%d", j + 1);

        const char* departments[ ] = { "内科", "外科", "儿科", "妇产科", "骨科", "眼科" };
        const char* titles[ ] = { "主任医师", "副主任医师", "主治医师", "住院医师" };
        int dept_index = j % (sizeof(departments) / sizeof(departments[0]));
        int title_index = (j / 5) % (sizeof(titles) / sizeof(titles[0])); // 假设每5个医生中有一个不同的职称  
        strcpy(doctors[j].department, departments[dept_index]);
        strcpy(doctors[j].title, titles[title_index]);
        strcpy(doctors[j].sex, (j % 2 == 0) ? "男" : "女");

        doctors[j].birth.year = 1970 + (j % 30); // 假设医生年龄分布在40-70岁之间  
        doctors[j].birth.month = (j % 12) + 1;
        doctors[j].birth.date = (j % 28) + 1; // 简化处理，不考虑月份天数差异  
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
        admins[k].id = 10000 + k + 1; // 假设ID从10001开始    
        snprintf(admins[k].name, sizeof(admins[k].name), "Admin%d", k + 1);
        snprintf(admins[k].password, sizeof(admins[k].password), "AdminPass%d", k + 1);
        snprintf(admins[k].phone, sizeof(admins[k].phone), "12345%05d", k + 1);
        strcpy(admins[k].department, "");
        strcpy(admins[k].title, "");
        strcpy(admins[k].sex, (k % 2 == 0) ? "男" : "女");
        admins[k].birth.year = 1980 + (k % 10);
        admins[k].birth.month = (k % 12) + 1;
        admins[k].birth.date = (k % 28) + 1; // 简化处理，不考虑月份天数差异    
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

    // 输入共同字段  
    printf("Enter ID: ");
    scanf("%lld", &new_user->id);
    setbuf(stdin, NULL);
    printf("Enter name: ");
    fgets(new_user->name, sizeof(new_user->name), stdin);
    new_user->name[strcspn(new_user->name, "\n")] = 0; // 移除fgets读取的换行符  

    printf("Enter password: ");
    fgets(new_user->password, sizeof(new_user->password), stdin);
    new_user->password[strcspn(new_user->password, "\n")] = 0; // 移除fgets读取的换行符  

    printf("Enter phone: ");
    fgets(new_user->phone, sizeof(new_user->phone), stdin);
    new_user->phone[strcspn(new_user->phone, "\n")] = 0; // 移除fgets读取的换行符  

    printf("Enter sex: ");
    fgets(new_user->sex, sizeof(new_user->sex), stdin);
    new_user->sex[strcspn(new_user->sex, "\n")] = 0; // 移除fgets读取的换行符  
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

    printf("请输入用户ID: ");
    scanf("%lld", &input_id);
    printf("请输入密码: ");
    scanf("%99s", input_password);

    USERS* temp = user_head;
    while (temp != NULL) {
        if (temp->id == input_id && strcmp(temp->password, input_password) == 0) {
            printf("登录成功!\n");
            return 1;
        }
        temp = temp->next;
    }

    printf("用户名或密码错误!\n");
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
            printf(Yellow("请输入要查找的用户ID: "));
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
            printf(Yellow("请输入要删除的用户ID: "));
            scanf("%lld", &id_to_delete);
            if (delete_user(id_to_delete) == 0) {
                printf(Yellow("用户删除成功\n"));
            }
            break;
        case 5:
            printf(Red("退出程序 \n"));
            return 0;
        default:
            printf(Red("Invalid choice, please try again.\n"));
        }
    }
    return 0;
}