#include "custom_c_string.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

char* CombineStrings(const int numStrings, int total_size, ...) {

    int writen_size = 0;
    char* result = calloc(total_size, sizeof(char));

    va_list args;
    va_start(args, numStrings);
    for(int i = 0; i < numStrings; i++) {
        const char* str = va_arg(args, const char*);
        const int str_length = strlen(str);
        if(writen_size + str_length > total_size - 1) {
            total_size *= 2;
            result = realloc(result, total_size);
        }
        strncpy(result + writen_size, str, str_length);
        writen_size += str_length;
    }

    va_end(args);
    EnsureNullOverTheBuffer(result, total_size);
    return result;
}


char* DuplicateString(const char* string) {
    const int lng = strlen(string);
    char* copy = calloc(lng+1, sizeof(char));
    strncpy(copy, string, lng);
    EnsureNullOverTheBuffer(copy, lng);
    return copy;
}

void EnsureNullOverTheBuffer(char* string, const int size) {
    const int buffer_length = strlen(string);
    for(int i = size; i < buffer_length; i++) {
        string[i] = '\0';
    }
}
