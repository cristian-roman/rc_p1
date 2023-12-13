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
#include "../url_handler/url_handler.h"
#include "../../algorithms/custom_boyer_moore/custom_boyer_moore.h"

struct Resources_Lenght_Pair {
    char** resources;
    int lenght;
};

const int NUMBER_OF_PATTERNS = 2;
const char* searching_patterns[] = {
    "src=\"",
    "href=\""
};

char* GetResource(const char* location) {
    int capacity = 10;
    char* resource = calloc(capacity, sizeof(char));
    int i = 0;
    while (location[i] != '"' && location[i] != '\0') {
        if (i == capacity) {
            capacity *= 2;
            resource = realloc(resource, capacity * sizeof(char));
        }
        resource[i] = location[i];
        i++;
    }

    while(i < capacity)
    {
        resource[i] = '\0';
        i++;
    }

    return resource;
}

int IsCharacterInvalid(const char c) {
    return c == ' ' ||
           c == '{' ||
           c == '}' ||
           c == '$' ||
           c == '[' ||
           c == ']';
}

int ContainsInvalidCharacters(const char* resource)
{
    for (size_t i = 0; i < strlen(resource); i++)
        {
            const char currentChar = resource[i];
            if (IsCharacterInvalid(currentChar)) {
                return 1;
            }
        }
    return 0;
}

int CheckForTreeFileStructure(const char* resource) {

    const size_t len = strlen(resource);
    for (size_t i = 1; i < len; i++) {

        //check agains double '/'
        if (resource[i] == '/' && resource[i - 1] == '/')
            return 0;
    }

    return 1;
}

int IsResourceValid(const char* resource)
{
    if(ContainsInvalidCharacters(resource))
        return 0;

    if(!CheckForTreeFileStructure(resource))
        return 0;
    return 1;
}

struct Resources_Lenght_Pair* ExtractRelativeUrls(const char* resource_path)
{
    FILE* file = fopen(resource_path, "r");
    if(file == NULL)
    {
        const char* pattern = "Failed to open file %s. URL Extracting interrupted";
        char* message = GetStringFromPattern(pattern, strlen(pattern) + strlen(resource_path) + 10, resource_path);
        LogError(message);
        free(message);
        return NULL;
    }

    struct Resources_Lenght_Pair* answer = malloc(sizeof(struct Resources_Lenght_Pair));
    answer->resources = malloc(sizeof(char*));
    answer->lenght = 0;

    char* line = NULL;
    size_t len = 0;
    // ReSharper disable once CppEntityAssignedButNoRead
    ssize_t read;
    // ReSharper disable once CppDFAUnusedValue
    int pattern_index;
    while ((read = getline(&line, &len, file)) != -1)
    {
        for(int i = 0; i < NUMBER_OF_PATTERNS; i++)
        {
            int position_after_pattern = 0;
            do {
                const char* line_after_pattern = line + position_after_pattern;
                 pattern_index = BoyerMooreSearch(line_after_pattern, searching_patterns[i]);
                if(pattern_index == -1)
                    continue;

                position_after_pattern = strstr(line_after_pattern, searching_patterns[i]) - line + strlen(searching_patterns[i]);
                char* resource = GetResource(line_after_pattern + position_after_pattern);

                if(!IsResourceValid(resource))
                {
                    free(resource);
                    continue;
                }

                answer->lenght++;
                answer->resources = realloc(answer->resources, (answer->lenght) * sizeof(char*));
                answer->resources[answer->lenght - 1] = resource;
            }while(pattern_index != -1);

        }
    }

    fclose(file);
    free(line);
    return answer;
}

int ResourceBeginsWithSlash(const char* resource) {
    return resource[0] == '/';
}

void AddResourcesToUrlTable(const struct UrlTable* url_table, const struct Folder_Resource_Pair* folder_resource_pair, const char* url)
{
    char* path_to_resource = CombineStrings(3, strlen(folder_resource_pair->folder) + strlen(folder_resource_pair->resource) + 2,
                                            folder_resource_pair->folder, "/", folder_resource_pair->resource);

    struct Resources_Lenght_Pair* resources_lenght_pair = ExtractRelativeUrls(path_to_resource);
    free(path_to_resource);

    char* url_without_resource = GetUrlWithoutResource(url);
    char* root_url = GetBaseUrl(url);
    for(int i = 0; i < resources_lenght_pair->lenght; i++)
    {
        char* resource = resources_lenght_pair->resources[i];
        char* new_url;
        if(ResourceBeginsWithSlash(resource))
        {
            new_url = CombineStrings(2,
                strlen(root_url) + strlen(resource),
                root_url,
                resource);
        }
        else
        {
            new_url = CombineStrings(3,
                strlen(url_without_resource) + strlen(resource) + 2,
                url_without_resource,
                "/",
                resource);
        }

        const int depth = GetUrlDepth(new_url);

        AddUrlToTable(url_table, new_url, depth);

        free(new_url);
        free(resource);
    }

    free(root_url);
    free(url_without_resource);
    free(resources_lenght_pair);
}