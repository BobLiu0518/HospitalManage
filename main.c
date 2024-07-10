#include <stdio.h>
#include <stdlib.h>
#include "beautifulDisplay.h"
#include "bed.h"
#include "medicine.h"
#include "record.h"
#include "registration.h"
#include "user.h"

USERS* currentUser = NULL;

void adminMain(USERS* currentUser) {
    int selection;
    while (1) {
        selection = displaySelect("欢迎使用管理员系统", -5, "用户管理", "挂号管理", "药品管理", "病床管理", "退出登录");
        switch (selection) {
        case -1:
        case 4:
            return;
        case 0:
            user_main(currentUser);
            break;
        case 1:
            medicineMain();
            break;
        case 2:
            bedMain();
            break;
        }
        system("pause > nul");
    }
}

void doctorMain() {
    int selection;
    while (1) {
        selection = displaySelect("欢迎使用医生系统", -5, "设置门诊坐诊时间", "查看患者信息", "添加病历、开药", "分配病房", "退出登录");
        switch (selection) {
        case -1:
        case 4:
            return;
        }
        system("pause > nul");
    }
}

void patientMain() {
    int selection;
    while (1) {
        selection = displaySelect("欢迎使用患者系统", -4, "预约挂号", "查看挂号记录", "查看历史病历", "退出登录");
        switch (selection) {
        case -1:
        case 3:
            return;
        case 0:
            assignRegistration(currentUser->id);
            break;
        case 1:
            // TODO
            break;
        case 2:
            checkHistoryRecord(currentUser->id);
            break;
        }
        system("pause > nul");
    }
}

int main() {
    int selection;
    create_data();
    system("chcp 936 > nul");
    system("title 医院管理系统");
    while (1) {
        selection = displaySelect("欢迎使用医院管理系统", -3, "登录系统", "患者注册", "退出系统");
        switch (selection) {
        case -1:
        case 2:
            printf("祝你生活愉快，再见！\n");
            system("pause > nul");
            exit(0);
            break;
        case 0:
            currentUser = login();
            if (!currentUser) {
                break;
            }
            switch (currentUser->user_type) {
            case 0:
                adminMain(currentUser);
                break;
            case 1:
                doctorMain();
                break;
            case 2:
                patientMain();
                break;
            }
            currentUser = NULL;
            break;
        case 1:
            create_user(0);
            break;
        }
        system("pause > nul");
    }
}