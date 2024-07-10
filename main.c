#include <stdio.h>
#include <stdlib.h>
#include "033.h"
#include "beautifulDisplay.h"
#include "bed.h"
#include "medicine.h"
#include "record.h"
#include "registration.h"
#include "user.h"

USERS* currentUser = NULL;

void adminMain() {
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
            // TODO
        case 2:
            medicineMain();
            break;
        case 3:
            bedMain();
            break;
        }
    }
}

void doctorMain() {
    int selection;
    while (1) {
        selection = displaySelect("欢迎使用医生系统", -4, "门诊坐诊时间管理", "查看患者信息", "填写病历", "退出登录");
        switch (selection) {
        case -1:
        case 3:
            return;
        case 0:
            selectClinicTime(currentUser->id);
            break;
        case 1:
            long long id;
            displayInput("输入患者就诊卡号", "%lld", &id);
            USERS* patient = find_user_by_id(id);
            if (!patient || patient->user_type != 2) {
                printf(Red("错误：")"患者 %lld 不存在。\n", id);
            } else {
                display_user_info(patient);
            }
            system("pause > nul");
            break;
        case 2:
            // TODO
            break;
        }
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
    }
}

int main() {
    int selection;
    // create_data();
    system("chcp 936 > nul");
    system("title 医院管理系统");
    while (1) {
        selection = displaySelect("欢迎使用医院管理系统", -3, "登录系统", "患者注册", "退出系统");
        switch (selection) {
        case 0:
            currentUser = login();
            if (!currentUser) {
                break;
            }
            switch (currentUser->user_type) {
            case 0:
                adminMain();
                break;
            case 1:
                doctorMain();
                break;
            case 2:
                patientMain();
                break;
            }
        case -1:
        case 2:
            if (currentUser && currentUser->user_type == 2) {
                printf("祝你早日康复，再见！\n");
            } else {
                printf("祝你生活愉快，再见！\n");
            }
            system("pause > nul");
            exit(0);
            break;
        case 1:
            create_user(0);
            system("pause > nul");
            break;
        }
    }
}