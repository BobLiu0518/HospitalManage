/*

#include <stdio.h>
#include <stdlib.h>
#include "beautifulDisplay.h"

int main() {
    system("chcp 936 > nul");
    int selection;
    char str[1000];
    displayInput("请输入用户名", "%s", str);
    printf("%s，你好！\n", str);
    displayInputPassword("请输入密码", str, 1000);
    printf("你的密码是 %s~", str);
    system("pause > nul");
    do {
        selection = displaySelect("请选择：", 5, "我是第一个选项", "我是第二个", "我应该是第三个吧", "那我就是第四个", "我是第五个");
        sprintf(str, "确定选择第 %d 个选项嘛？", selection + 1);
        selection = displaySelect(str, 2, "是", "否");
    } while (selection);
    printf("好的。\n");

    system("pause > nul");
    return 0;
}

*/