#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "passwordTools.h"
#include "md5.h" // Source: https://sourceforge.net/projects/libmd5-rfc/

void encryptPassword(char* source, char* target) {
    int i;
    char* str;
    md5_state_t state;
    md5_byte_t digest[16];
    str = malloc(strlen(source) + strlen(ENCRYPT_SALT) + 1);
    strcpy(str, source);
    strcat(str, ENCRYPT_SALT);

    // ≤Œ’’ libmd5-rfc/md5main/md5main.c
    md5_init(&state);
    md5_append(&state, str, strlen(str));
    md5_finish(&state, digest);
    for (i = 0; i < 16; ++i) {
        sprintf(target + i * 2, "%02x", digest[i]);
    }
    target[32] = '\0';
    return;
}