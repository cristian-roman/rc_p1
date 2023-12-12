//
// Created by cristian on 12.12.2023.
//

#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "../../../custom_libraries/custom_c_logger/custom_c_logger.h"


short IsURLValid(const char* url) {
    if (strncmp(url, "http://", 7) == 0) {
        return 1;
    }

    if (strncmp(url, "https://", 8) == 0) {
        return 1;
    }

    return 0;
}

char* ReadURL()
{
    const short MAX_URL_LENGTH = 256;
    char* url = malloc(MAX_URL_LENGTH * sizeof(char));
    bzero(url, MAX_URL_LENGTH);
    char c;
    int characters_read = 0;
    while(c != ' ' && c != '\n') {
        c = getchar();
        strncat(url, &c, 1);
        characters_read++;
        if (characters_read == MAX_URL_LENGTH) {
            break;
        }
    }

    while (c != '\n') {
        c = getchar();
    }

    url[strcspn(url, "\n")] = '\0';

    const size_t url_length = strlen(url);
    if (url_length > 0 && url[url_length - 1] == '/') {
        LogWarning("Trailing slash detected in URL. If your intention was without the trailing slash, please restart the client and type the URL again without the trailing slash");
    }

    return url;
}

int ReadDepth()
{
    int depth;
    if (scanf("%d", &depth) != 1) {
        return -1;
    }
    getchar();

    return depth;
}