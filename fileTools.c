#include <stdio.h>
#include <string.h>
#include "033.h"

int fileOpen(FILE** fp, char* path, char* mode) {
    int i;
    *fp = fopen(path, mode);
    if (!*fp) {
        if (mode[0] == 'r') {
            printf(Yellow("警告：")"读取文件 %s 失败，尝试创建空白文件。\n", path);
            *fp = fopen(path, "w+");
            if (!*fp) {
                printf(Red("错误：")"创建文件 %s 失败。\n", path);
                return -1;
            }
        } else {
            printf(Red("错误：")"创建文件 %s 失败。\n", path);
            return -1;
        }
    }
    return 0;
}