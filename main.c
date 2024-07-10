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
        selection = displaySelect("��ӭʹ�ù���Աϵͳ", -5, "�û�����", "�ҺŹ���", "ҩƷ����", "��������", "�˳���¼");
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
        selection = displaySelect("��ӭʹ��ҽ��ϵͳ", -5, "������������ʱ��", "�鿴������Ϣ", "��Ӳ�������ҩ", "���䲡��", "�˳���¼");
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
        selection = displaySelect("��ӭʹ�û���ϵͳ", -4, "ԤԼ�Һ�", "�鿴�Һż�¼", "�鿴��ʷ����", "�˳���¼");
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
    system("title ҽԺ����ϵͳ");
    while (1) {
        selection = displaySelect("��ӭʹ��ҽԺ����ϵͳ", -3, "��¼ϵͳ", "����ע��", "�˳�ϵͳ");
        switch (selection) {
        case -1:
        case 2:
            printf("ף��������죬�ټ���\n");
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