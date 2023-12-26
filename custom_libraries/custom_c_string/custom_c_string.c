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
        if(writen_size + strlen(str) > total_size) {
            total_size *= 2;
            result = realloc(result, total_size);
        }
        strcpy(result + writen_size, str);
        writen_size += strlen(str);
    }

    va_end(args);
    return result;
}

char* GetStringFromPattern(const char* pattern, const int total_size, ...) {

    char* string = calloc(total_size + 10, sizeof(char));

    va_list args;
    va_start(args, total_size);
    vsnprintf(string, total_size, pattern, args);
    va_end(args);

    return string;
}
