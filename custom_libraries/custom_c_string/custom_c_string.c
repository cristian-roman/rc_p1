#include "custom_c_string.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

char* CombineStrings(const int numStrings, const int total_size, ...) {

    char* result = malloc(total_size);
    sprintf(result, "");

    va_list args;
    va_start(args, numStrings);
    for(int i = 0; i < numStrings; i++) {
        const char* str = va_arg(args, const char*);
        strcat(result, str);
    }

    va_end(args);
    return result;
}

char* GetStringFromPattern(const char* pattern, const int total_size, ...) {

    char* string = malloc(total_size);
    sprintf(string, "");

    va_list args;
    va_start(args, total_size);
    vsnprintf(string, total_size, pattern, args);
    va_end(args);

    return string;
}
