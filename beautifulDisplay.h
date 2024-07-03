#ifndef BEAUTIFUL_DISPLAY
#define BEAUTIFUL_DISPLAY

int displayInput(char* prompt, char* format, void* ptr);
int displayInputPassword(char* prompt, char* result, int max);
int displaySelect(char* prompt, int count, ...);

#endif