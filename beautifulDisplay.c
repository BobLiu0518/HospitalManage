#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>
#include "033.h"

int displayInput(char* prompt, char* format, void* ptr) {
    int i;
    printf("\033[7m %s: \033[s", prompt); // ��ɫ & ������λ��
    for (i = strlen(prompt); i < 37; i++) {
        printf(" ");
    }
    printf("\033[u"); // ���ù��λ��
    i = scanf(format, ptr);
    printf("\033[0m"); // ���������ʽ
    return i;
}

int displayInputMultiline(char* prompt, char* str, int max) {
    int i, length = 0;
    char* tmp = calloc(max, sizeof(char));
    str[0] = '\0';
    printf("\033[7m %s: ", prompt); // ��ɫ
    for (i = strlen(prompt); i < 37; i++) {
        printf(" ");
    }
    printf("\n");
    getchar(); // �Ե����з�

    do {
        printf(" > \033[s"); // ������λ��
        for (i = 0; i < 37; i++) {
            printf(" ");
        }
        printf("\033[u"); // ���ù��λ��
        fgets(tmp, max, stdin);
        if (length + strlen(tmp) > max) {
            printf(Yellow("���棺")"�Ѿ������������ƣ��������ݻᱻ�ضϡ�\n");
            break;
        }
        length += strlen(tmp);
        strcat(str, tmp);
    } while (tmp[0] != '\n' && tmp[0] != '\r');

    printf("\b\033[0m\n"); // ���������ʽ
    return length;
}

int displayInputPassword(char* prompt, char* result, int max) {
    int i;
    char c;
    printf("\033[7m %s: \033[s", prompt); // ��ɫ & ������λ��
    for (i = strlen(prompt); i < 37; i++) {
        printf(" ");
    }
    printf("\033[u"); // ���ù��λ��
    i = 0;
    while (1) {
        c = getch();
        if (c == '\b') {
            if (i > 0) {
                result[i] = '\0';
                i--;
                printf("\b \b");
            } else {
                printf("\a");
            }
        } else {
            if (isprint(c)) {
                if (i >= max - 1) {
                    printf("\a");
                } else {
                    result[i] = c;
                    i++;
                    printf("*");
                }
            } else {
                result[i] = '\0';
                i++;
                break;
            }
        }
    }
    printf("\033[0m\n"); // ���������ʽ
    return i;
}

int displaySelect(char* prompt, int count, ...) {
    int i, j, selection = 0, input;
    va_list argList;
    char** options = calloc(count, sizeof(char*));
    va_start(argList, count);
    for (i = 0; i < count; i++) {
        options[i] = va_arg(argList, char*);
    }

    printf("\033[?25l\033[2J"); // ���ع�� & ����
    while (1) {
        printf("\033[1;1H"); // ���ù��λ��

        // ��ʾ prompt
        if (strlen(prompt)) {
            printf("%s\n", prompt);
        }

        // ��ʾѡ��
        for (i = 0; i < count; i++) {
            if (selection == i) {
                printf("\033[7m"); // ��ɫ
            }
            printf(" [%d] %s", i + 1, options[i]);
            for (j = strlen(options[i]); j < 35; j++) {
                printf(" ");
            }
            printf("\033[0m\n"); // ���������ʽ
        }
        // printf("---------------------------------------");

        // ������
        input = getch();
        if (input >= '1' && input <= '9' && input - '1' < count) {
            selection = input - '1';
        } else if (input == 72 /* �ϼ�ͷ */ || input == 'w' || input == 'W') {
            selection = (selection + count - 1) % count;
        } else if (input == 80 /* �¼�ͷ */ || input == 9 /* Tab */ || input == 's' || input == 'S') {
            selection = (selection + 1) % count;
        } else if (input == 27 /* Esc */) {
            selection = -1;
            break;
        } else if (input == '\r' || input == '\n' || input == ' ') {
            break;
        }
    }

    printf("\033[1;1H\033[2J\033[?25h"); // ���ù��λ�� & ���� & ��ʾ���
    fflush(stdin);
    va_end(argList);
    free(options);
    return selection;
}