//
// Created by cristian on 12.12.2023.
//

#include "url_extractor.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../../../../custom_libraries/custom_c_logger/custom_c_logger.h"
#include "../../../../custom_libraries/custom_c_string/custom_c_string.h"

const int NUMBER_OF_PATTERNS = 2;

void HandleOpenFileError(const char* resource_path){
    const char* pattern = "Failed to open file %s. URL Extracting interrupted";
    char* message = GetStringFromPattern(pattern, strlen(pattern) + strlen(resource_path) + 10, resource_path);
    LogError(message);
    free(message);
}

char** InitializePatternsToSearchFor() {

    char** patterns = malloc(NUMBER_OF_PATTERNS * sizeof(char*));

    const char* src_pattern = "src=\"";
    patterns[0] = malloc(strlen(src_pattern) + 1);
    strcpy(patterns[0], src_pattern);

    const char* href_pattern = "href=\"";
    patterns[1] = malloc(strlen(href_pattern) + 1);
    strcpy(patterns[1], href_pattern);

    return patterns;
}

void FreePatterns(char** patterns) {
    free(patterns[0]);
    free(patterns[1]);
    free(patterns);
}

char* FillBufferWithTag(FILE * file) {
    int capacity = 256;
    int size = 0;
    char* buffer = malloc(capacity * sizeof(char));
    char character;

    if (buffer == NULL) {
        LogError("Bad allocation for buffer in FillBufferWithTag");
        return NULL;
    }

    buffer[size++] = '<';

    do {
        character = fgetc(file);

        if (size >= capacity - 1) {
            capacity *= 2;
            buffer = realloc(buffer, capacity * sizeof(char));

            if (buffer == NULL) {
                LogError("Bad allocation for buffer in FillBufferWithTag");
                return NULL;
            }
        }

        buffer[size++] = character;

    } while (character != EOF && character != '>');

    return buffer;
}


int TagContainsResource(const char* tag, char** patterns) {
    int ans = 0;
    for(int i = 0; i < NUMBER_OF_PATTERNS; i++) {
        if(strstr(tag, patterns[i]) != NULL) {
            ans = 1;

            break;
        }
    }

    return ans;
}

int CheckForValidCharacters(const char* resource_name) {

    for (size_t i = 0; i < strlen(resource_name); i++) {
        const char currentChar = resource_name[i];
        if (!isalnum(currentChar) && currentChar != '/' && currentChar != '.') {
            return 0;
        }
    }

    return 1;
}

int CheckForTreeFileStructure(const char* resource_name) {
    const size_t len = strlen(resource_name);
    if (resource_name[0] != '/' && !isalnum(resource_name[0])) {
        return 0;
    }

    for (size_t i = 1; i < len; i++) {
        if (resource_name[i] == '/' && resource_name[i - 1] == '/') {
            return 0;
        }

        if (i < len - 1 && resource_name[i] == '/' && !isalnum(resource_name[i + 1])) {
            return 0;
        }
    }

    return 1;
}

int IsResourceNameValid(const char* resource_name) {

    if(CheckForValidCharacters(resource_name) == 0) {
        return 0;
    }

    if(CheckForTreeFileStructure(resource_name) == 0) {
        return 0;
    }

    return 1;
}

char* ExtractResourceNameFromTag(const char* tag, const char** patterns) {

    const char* start_pointer = NULL;
    int length = 0;

    for (int i = 0; i < NUMBER_OF_PATTERNS; i++) {
        start_pointer = strstr(tag, patterns[i]);
        if (start_pointer != NULL) {
            length = strlen(patterns[i]);
            break;
        }
    }

    if (start_pointer == NULL) {
        const char* pattern = "Failed to extract resource name from tag: %s";
        char* message = GetStringFromPattern(pattern, strlen(pattern) + strlen(tag) + 10, tag);
        LogError(message);
        free(message);
        return NULL;
    }

    const int start_index = start_pointer - tag + length;

    int index = start_index;
    while (tag[index] != '\"') {
        index++;
    }

    const int required_capacity = index - start_index + 1;
    char* resource_name = malloc(required_capacity * sizeof(char));

    strncpy(resource_name, tag + start_index, index - start_index);
    resource_name[index - start_index] = '\0';
    return resource_name;
}


void AddStringToArray(char*** answer, const char* resource_name, const int resource_number) {
    if (*answer == NULL) {
        *answer = malloc(sizeof(char*));
    } else {
        *answer = realloc(*answer, (resource_number+1) * sizeof(char*));
    }

    (*answer)[resource_number] = malloc(strlen(resource_name) + 1);
    strcpy((*answer)[resource_number], resource_name);
}

char** ExtractResourcesNames(const char* resource_path, int *number_of_resources_found)
{
    FILE* file = fopen(resource_path, "r");
    if(file == NULL)
    {
        HandleOpenFileError(resource_path);
        return NULL;
    }

    char** answer = NULL;
    *number_of_resources_found = 0;

    char** patterns = InitializePatternsToSearchFor();

    char character;
    do {
        character = fgetc(file);
        if(character == '<') {
            char* tag = FillBufferWithTag(file);
            if(TagContainsResource(tag, patterns)) {
                char* resource_name = ExtractResourceNameFromTag(tag, patterns);
                if(IsResourceNameValid(resource_name)) {
                    AddStringToArray(&answer, resource_name, *number_of_resources_found);
                    *number_of_resources_found += 1;
                }
                free(resource_name);
            }
            free(tag);
        }
    }while(character != EOF);

    FreePatterns(patterns);
    fclose(file);

    return answer;
}

void FreeResources(char** resources_names, const int number_of_resources_found) {
    for(int i = 0; i < number_of_resources_found; i++) {
        free(resources_names[i]);
    }
    free(resources_names);
}

int IsURLAlreadyIncluded(const char** answer, const int refferenced_urls_count, const char* new_url ) {
    for(int i = 0; i < refferenced_urls_count; i++) {
        if(strcmp(answer[i], new_url) == 0) {
            return 1;
        }
    }
    return 0;
}

char* GetRootUrl(const char* url) {
    char* root_url;
    const char* lookup = url;
    const char* third_slash = NULL;

    for(int i = 0 ; i < 3; i++) {
        third_slash = strchr(lookup, '/');
        if(third_slash == NULL) {
            break;
        }

        lookup = third_slash + 1;
    }

    if(third_slash == NULL) {
        root_url = strdup(url);
    }
    else {
        const int index = third_slash - url;
        root_url = malloc(index + 1);
        strncpy(root_url, url, index);
        root_url[index] = '\0';
    }

    return root_url;
}

char** ExtractReferencedURLs(const char** resources_names, const int number_of_resources_found, const char* url, int* refferenced_urls_count) {

    *refferenced_urls_count = 0;
    char** answer = malloc(sizeof(char*));
    char* root_url = GetRootUrl(url);

    for(int i = 0; i < number_of_resources_found; i++) {
        if(resources_names[i][0] == '/') {
            char* new_url = CombineStrings(2, strlen(root_url) + strlen(resources_names[i]), root_url, resources_names[i]);
            if(IsURLAlreadyIncluded(answer, *refferenced_urls_count, new_url) == 0) {
                answer = realloc(answer, (*refferenced_urls_count + 1) * sizeof(char*));
                answer[*refferenced_urls_count] = new_url;
                *refferenced_urls_count += 1;
            }
        }
    }

    free(root_url);
    return answer;
}

int IsResourceOnLevel(const char* resource_name) {
    for(int i = 0; i < strlen(resource_name); i++) {
        if(resource_name[i] == '/') {
            return 0;
        }
    }
    return 1;
}

char** ExtractOnLevelURLs(const char** resources_names, const int number_of_resources_found, const char* url, int* on_level_urls_count) {
    *on_level_urls_count = 0;
    char** answer = malloc(sizeof(char*));

    for(int i = 0; i < number_of_resources_found; i++) {
        if(IsResourceOnLevel(resources_names[i])) {
            char* new_url = CombineStrings(3, strlen(url) + strlen(resources_names[i]) + 2, url, "/", resources_names[i]);
            if(IsURLAlreadyIncluded(answer, *on_level_urls_count, new_url) == 0) {
                answer = realloc(answer, (*on_level_urls_count + 1) * sizeof(char*));
                answer[*on_level_urls_count] = new_url;
                *on_level_urls_count += 1;
            }
        }
    }

    return answer;
}


