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


int GetNumberOfTokens(const char* str, const char delimiter) {
    int result = 0;
    for(int i = 0; i < strlen(str); i++) {
        if(str[i] == delimiter) {
            result++;
        }
    }

    if(str[strlen(str)-1] == '/')
        result--;

    return result + 1;
}

char** SplitString(char* str, const char delimiter, const int numTokens) {

    char** result = malloc(numTokens * sizeof(char*));
    bzero(result, numTokens * sizeof(char*));
    const char* token = strtok(str, &delimiter);
    int i = 0;
    while (token != NULL && i < numTokens) {
        result[i] = malloc(strlen(token) + 2);
        bzero(result[i], strlen(token) + 2);
        strcpy(result[i], token);
        token = strtok(NULL, &delimiter);
        i++;
    }

    return result;
}

void FreeSplitString(char** tokens, const int numTokens) {
    for (int i = 0; i < numTokens; i++) {
        free(tokens[i]);
    }
    free(tokens);
}

