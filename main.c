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
        selection = displaySelect("��ӭʹ�ù���Աϵͳ", -5, "�û�����", "�ҺŹ���", "ҩƷ����", "��������", "�˳���¼");
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
        selection = displaySelect("��ӭʹ��ҽ��ϵͳ", -4, "��������ʱ�����", "�鿴������Ϣ", "��д����", "�˳���¼");
        switch (selection) {
        case -1:
        case 3:
            return;
        case 0:
            selectClinicTime(currentUser->id);
            break;
        case 1:
            long long id;
            displayInput("���뻼�߾��￨��", "%lld", &id);
            USERS* patient = find_user_by_id(id);
            if (!patient || patient->user_type != 2) {
                printf(Red("����")"���� %lld �����ڡ�\n", id);
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
    }
}

int main() {
    int selection;
    // create_data();
    system("chcp 936 > nul");
    system("title ҽԺ����ϵͳ");
    while (1) {
        selection = displaySelect("��ӭʹ��ҽԺ����ϵͳ", -3, "��¼ϵͳ", "����ע��", "�˳�ϵͳ");
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
                printf("ף�����տ������ټ���\n");
            } else {
                printf("ף��������죬�ټ���\n");
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