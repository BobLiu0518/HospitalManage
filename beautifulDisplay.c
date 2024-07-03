#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>

int displayInput(char* prompt, char* result, int max, char password) {
    int i;
    char c;
    printf("\033[7m %s: \033[s", prompt); // 反色 & 保存光标位置
    for (i = strlen(prompt); i < 37; i++) {
        printf(" ");
    }
    printf("\033[u"); // 重置光标位置
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
                    printf("%c", password ? '*' : c);
                }
            } else {
                result[i] = '\0';
                i++;
                break;
            }
        }
    }
    printf("\033[0m\n"); // 重置输出格式
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
                printf("\033[7m"); // 反色
            }
            printf(" [%d] %s", i + 1, options[i]);
            for (j = strlen(options[i]); j < 35; j++) {
                printf(" ");
            }
            printf("\033[0m\n"); // 重置输出样式
        }
        // printf("---------------------------------------");

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
    fflush(stdin);
    va_end(argList);
    free(options);
    return selection;
}