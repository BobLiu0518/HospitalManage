#ifndef BEAUTIFUL_DISPLAY
#define BEAUTIFUL_DISPLAY

int displayTitle(char* title);
int displayInput(char* prompt, char* format, void* ptr);
int displayInputMultiline(char* prompt, char* str, int max);
int displayInputPassword(char* prompt, char* result, int max);
int displaySelect(char* prompt, int count, ...);

#endif