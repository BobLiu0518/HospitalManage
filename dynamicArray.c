#include <stdio.h>
#include <stdlib.h>
#include "033.h"
#include "dynamicArray.h"

int initDynamicArray(DynamicArray* arr, unsigned capacity, size_t item_size) {
    arr->ptr = calloc(capacity, item_size);
    if (!arr->ptr) {
        printf(Red("����")"��̬�����ʼ��ʧ�ܡ�\n");
        return -1;
    }
    arr->length = 0;
    arr->capacity = capacity;
    arr->item_size = item_size;
    return 0;
}

int appendItem(DynamicArray* arr, void* item) {
    if (arr->length <= 0 || !arr->ptr) {
        printf(Red("����")"��̬����δ��ʼ����\n");
        return -1;
    }
    if (arr->length == arr->capacity) {
        arr->capacity *= 2;
        arr->ptr = realloc(arr->ptr, arr->capacity * arr->item_size);
        if (!arr->ptr) {
            printf(Red("����")"��̬��������ʧ�ܡ�\n");
            return -1;
        }
    }
    void* newItem = (char*)arr->ptr + arr->length * arr->item_size;
    memcpy(newItem, item, arr->item_size);
    arr->length++;
    return 0;
}

void* getItem(DynamicArray* arr, int index) {
    if (index < 0 || index >= arr->length) {
        return NULL;
    }
    return (char*)arr->ptr + index * arr->item_size; // char ��СΪ 1 �ֽ�
}

void freeDynamicArray(DynamicArray* arr) {
    free(arr->ptr);
    arr->ptr = NULL;
    arr->length = 0;
    arr->capacity = 0;
}