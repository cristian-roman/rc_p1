//
// Created by cristian on 13.12.2023.
//

#include "url_handler.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "../../../../custom_libraries/custom_c_logger/custom_c_logger.h"
#include "../../../../custom_libraries/custom_c_string/custom_c_string.h"

char* RemoveUrlPrefix(const char* url)
{
    char* url_without_copy = 0;

    if(strncmp(url, "http://", 7) == 0)
    {
        url_without_copy = (char*)malloc(strlen(url) - 6);
        strcpy(url_without_copy, url + 7);
    }
    else if(strncmp(url, "https://", 8) == 0)
    {
        url_without_copy = (char*)malloc(strlen(url) - 7);
        strcpy(url_without_copy, url + 8);
    }
    else
    {
        const char* pattern = "Prefix extractor failed for link: %s";
        char* message = GetStringFromPattern(pattern, strlen(pattern) + strlen(url) + 10, url);
        LogError("Prefix extractor failed for link: %s");
        free(message);
    }

    return url_without_copy;
}


char* RemoveLastSlash(const char* url)
{
    char* url_copy = strdup(url);

    const int len = strlen(url_copy);
    if(url_copy[len - 1] == '/')
    {
        url_copy[len - 1] = '\0';
    }

    return url_copy;
}


struct Tokens_Length_Pair* ExtractUrlTokens(const char* url) {
    struct Tokens_Length_Pair* tokens_length_pair = malloc(sizeof(struct Tokens_Length_Pair));
    tokens_length_pair->tokens = malloc(sizeof(char*));
    tokens_length_pair->length = 0;

    char* url_copy = strdup(url);
    const char* token = strtok(url_copy, "/");

    while(token != NULL) {
        tokens_length_pair->length += 1;
        tokens_length_pair->tokens = realloc(tokens_length_pair->tokens, tokens_length_pair->length * sizeof(char*));
        tokens_length_pair->tokens[tokens_length_pair->length - 1] = strdup(token);
        token = strtok(NULL, "/");
    }

    free(url_copy);
    return tokens_length_pair;
}

void FreeTokensLengthPair(const struct Tokens_Length_Pair* tokens_length_pair) {
    for(int i = 0; i < tokens_length_pair->length; i++) {
        free(tokens_length_pair->tokens[i]);
    }
    free(tokens_length_pair->tokens);
}

int TokenHasExtension(const char* token) {
    const char* dot_location = strrchr(token, '.');
    if(dot_location == NULL) {
        return 0;
    }

    const int len = strlen(dot_location+1);
    if(len >= 1 && len <= 5) {
        for (int i = 1; i < len; i++) {
            if (!isalpha(dot_location[i])) {
                return 0;
            }
        }
        return 1;
    }

    return 0;
}

char* GetUrlWithoutResource(const char* url) {
    char* url_without_resource = strdup(url);

    if(!TokenHasExtension(url_without_resource)) {
        if(url_without_resource[strlen(url_without_resource) - 1] == '/') {
            url_without_resource[strlen(url_without_resource) - 1] = '\0';
        }
        return url_without_resource;
    }
    int len = 0;
    if(strstr(url_without_resource, "http://") != NULL) {
        len = strlen("http://");
    }
    else if(strstr(url_without_resource, "https://") != NULL) {
        len = strlen("https://");
    }

    const char* last_slash = strrchr(url_without_resource+len, '/');
    if(last_slash != NULL)
        {
            int index = last_slash - url_without_resource;
            len = strlen(url_without_resource);

            while(index < len)
            {
                url_without_resource[index] = '\0';
                index += 1;
            }
        }
    return url_without_resource;
}

int IsUrl(const char* str) {
    if(strstr(str, "http://") != NULL || strstr(str, "https://") != NULL) {
        return 1;
    }
    return 0;
}

char* GetBaseUrl(const char* url) {
    int len = 0;
    if(strstr(url, "http://") != NULL) {
        len = strlen("http://");
    }
    else if(strstr(url, "https://") != NULL) {
        len = strlen("https://");
    }

    const char* first_slash = strchr(url+len, '/');
    char* root_url;
    if(first_slash != NULL) {
        len = first_slash-url;
        root_url = malloc(len);
        strncpy(root_url, url, len);
    }
    else {
        root_url = strdup(url);
    }

    return root_url;
}

int GetUrlDepth(const char* url) {

    char* url_without_resource = GetUrlWithoutResource(url);
    char* unprefixed_url_without_resource = RemoveUrlPrefix(url_without_resource);
    free(url_without_resource);

    const size_t len = strlen(unprefixed_url_without_resource);
    int number_of_slashes = 0;

    for(int i = 0; i < len; i++) {
        if(unprefixed_url_without_resource[i] == '/') {
            number_of_slashes += 1;
        }
    }

    free(unprefixed_url_without_resource);
    return number_of_slashes;
}
