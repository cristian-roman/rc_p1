#include "myString.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

void CombineStrings(char* result, const int numStrings, ...) {
    sprintf(result, "");

    va_list args;
    va_start(args, numStrings);
    for(int i = 0; i < numStrings; i++) {
        const char* str = va_arg(args, const char*);
        strcat(result, str);
    }

    va_end(args);
}

void ReverseIntegerDigitOrder(char str[], const int length) {
    int start = 0;
    int end = length - 1;
    while (start < end) {
        const char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

char* IntegerToString(char* result, int number)
{
    int i = 0;
    int isNegative = 0;

    // Handle 0 explicitly, otherwise empty string is printed
    if (number == 0) {
        result[i++] = '0';
        result[i] = '\0';
        return result;
    }

    // Handle negative numbers only if base is 10
    if (result < 0) {
        isNegative = 1;
    }

    while (number != 0) {
        const int base = 10;
        const int rem = number % base;
        result[i++] = (char)(rem + (int)'0');
        number = number / base;
    }

    if (isNegative)
        result[i++] = '-';

    result[i] = '\0'; // Null-terminate string

    ReverseIntegerDigitOrder(result, i);

    return result;
}

char** SplitString(char* str, const char delimiter, const int numTokens) {
    char** result = malloc(numTokens * sizeof(char*));
    bzero(result, numTokens * sizeof(char*));
    const char* token = strtok(str, &delimiter);
    int i = 0;
    while (token != NULL) {
        result[i] = malloc(strlen(token) + 2);
        bzero(result[i], strlen(token) + 2);
        strcpy(result[i], token);
        token = strtok(NULL, &delimiter);
        i++;
    }

    return result;
}
