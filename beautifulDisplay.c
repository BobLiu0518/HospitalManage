#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>
#include "033.h"

int displayTitle(char* title) {
    int i = 0;
    printf("\033[2J\033[7m");
    while (i < 40) {
        if (i == (40 - strlen(title)) / 2 - 1) {
            printf("%s", title);
            i += strlen(title);
        } else {
            printf(" ");
            i++;
        }
    }
    printf("\033[0m\n");
}

int displayInput(char* prompt, char* format, void* ptr) {
    int i;
    printf("%s: \033[2m", prompt);
    i = scanf(format, ptr);
    printf("\033[0m");
    return i;
}

int displayInputMultiline(char* prompt, char* str, int max) {
    int length = 0;
    char* tmp = calloc(max, sizeof(char));
    str[0] = '\0';
    printf("%s:\n", prompt);
    setbuf(stdin, NULL); // 清空 stdin，不知道为什么 fflush 不起作用

    do {
        printf("> \033[2m");
        fgets(tmp, max, stdin);
        printf("\033[0m");
        if (length + strlen(tmp) > max) {
            printf(Yellow("警告：")"已经超出长度限制，输入内容会被截断。\n");
            break;
        }
        length += strlen(tmp);
        strcat(str, tmp);
    } while (tmp[0] != '\n' && tmp[0] != '\r');

    printf("\n");
    return length;
}

int displayInputPassword(char* prompt, char* result, int max) {
    int i;
    char c;
    printf("%s: \033[2m", prompt);
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
    printf("\033[0m\n");
    return i;
}

int displaySelect(char* prompt, int count, ...) {
    int i, j, selection = 0, input, memoryAllocated = 0;
    va_list argList;
    char** options;
    va_start(argList, count);
    if (count < 0) {
        count = -count;
        memoryAllocated = 1;
        options = calloc(count, sizeof(char*));
        for (i = 0; i < count; i++) {
            options[i] = va_arg(argList, char*);
        }
    } else {
        options = va_arg(argList, char**);
    }

    printf("\033[?25l\033[2J"); // 隐藏光标 & 清屏
    while (1) {
        printf("\033[1;1H"); // 重置光标位置

        // 显示 prompt
        if (strlen(prompt)) {
            printf("%s\n", prompt);
        }

        // 显示选项
        for (i = 0; i < count; i++) {
            if (selection == i) {
                printf("\033[30;47m"); // 反色
            }
            printf(" [%d] %s", i + 1, options[i]);
            for (j = strlen(options[i]); j < 35; j++) {
                printf(" ");
            }
            printf("\033[0m\n"); // 重置输出样式
        }

        // 处理按键
        input = getch();
        if (input >= '1' && input <= '9' && input - '1' < count) {
            selection = input - '1';
        } else if (input == 72 /* 上箭头 */ || input == 'w' || input == 'W') {
            selection = (selection + count - 1) % count;
        } else if (input == 80 /* 下箭头 */ || input == 9 /* Tab */ || input == 's' || input == 'S') {
            selection = (selection + 1) % count;
        } else if (input == 27 /* Esc */) {
            selection = -1;
            break;
        } else if (input == '\r' || input == '\n' || input == ' ') {
            break;
        }
    }

    printf("\033[1;1H\033[2J\033[?25h"); // 重置光标位置 & 清屏 & 显示光标
    if (memoryAllocated) {
        free(options);
    }
    setbuf(stdin, NULL);
    va_end(argList);
    return selection;
}