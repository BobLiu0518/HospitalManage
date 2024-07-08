#include <stdio.h>
#include <string.h>
#include "033.h"

int fileOpen(FILE** fp, char* path, char* mode) {
    int i;
    *fp = fopen(path, mode);
    if (!*fp) {
        if (mode[0] == 'r') {
            printf(Yellow("���棺")"��ȡ�ļ� %s ʧ�ܣ����Դ����հ��ļ���\n", path);
            *fp = fopen(path, "w+");
            if (!*fp) {
                printf(Red("����")"�����ļ� %s ʧ�ܡ�\n", path);
                return -1;
            }
        } else {
            printf(Red("����")"�����ļ� %s ʧ�ܡ�\n", path);
            return -1;
        }
    }
    return 0;
}