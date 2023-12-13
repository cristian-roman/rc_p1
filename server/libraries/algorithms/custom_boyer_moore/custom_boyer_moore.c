//
// Created by cristian on 13.12.2023.
//

#include "custom_boyer_moore.h"

#include <string.h>

int BoyerMooreSearch(const char* text, const char* pattern) {
    const int n = strlen(text);
    const int m = strlen(pattern);

    int bad_char[256];
    for (int i = 0; i < 256; i++) {
        bad_char[i] = m;
    }
    for (int i = 0; i < m - 1; i++) {
        bad_char[(unsigned char) pattern[i]] = m - 1 - i;
    }

    int i = m - 1;
    int j = m - 1;

    while (i < n) {
        if (text[i] == pattern[j]) {
            if (j == 0)
                return i;
            i--;
            j--;
        }
        else {
            i += bad_char[(unsigned char) text[i]];
            j = m - 1;
        }
    }

    return -1;
}
