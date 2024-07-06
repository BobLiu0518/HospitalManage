#ifndef DYNAMIC_ARRAY
#define DYNAMIC_ARRAY

typedef struct {
    void* ptr;
    unsigned length;
    unsigned capacity;
    size_t item_size;
} DynamicArray;

int initDynamicArray(DynamicArray* arr, unsigned capacity, size_t item_size);
int appendItem(DynamicArray* arr, void* item);
void* getItem(DynamicArray* arr, int index);
void freeDynamicArray(DynamicArray* arr);

#endif