/*

#include <stdio.h>
#include <stdlib.h>
#include "beautifulDisplay.h"

int main() {
    system("chcp 936 > nul");
    int selection;
    char str[1000];
    displayInput("�������û���", "%s", str);
    printf("%s����ã�\n", str);
    displayInputPassword("����������", str, 1000);
    printf("��������� %s~", str);
    system("pause > nul");
    do {
        selection = displaySelect("��ѡ��", 5, "���ǵ�һ��ѡ��", "���ǵڶ���", "��Ӧ���ǵ�������", "���Ҿ��ǵ��ĸ�", "���ǵ����");
        sprintf(str, "ȷ��ѡ��� %d ��ѡ���", selection + 1);
        selection = displaySelect(str, 2, "��", "��");
    } while (selection);
    printf("�õġ�\n");

    system("pause > nul");
    return 0;
}

*/