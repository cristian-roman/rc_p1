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

char* GetResource(const char* page, const char* location) {
    int capacity = 10;
    char* resource = calloc(capacity, sizeof(char));
    int i = 0;
    int index = location - page;
    while (index < 4096 && location[i] != '"' && location[i] != '\0') {
        if (i == capacity) {
            capacity *= 2;
            resource = realloc(resource, capacity * sizeof(char));
            bzero(resource + i, capacity / 2);
        }
        resource[i] = location[i];
        i++;
        index++;
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
           c == ']' ||
           c == '=' ||
           c < 32 ||
           c > 126;
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
    if(resource[0] == '?') {
        return 0;
    }

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
        char* message = calloc(strlen(pattern) + strlen(resource_path) + 10, sizeof(char));
        LogError(message);
        free(message);
        return NULL;
    }

    struct Resources_Lenght_Pair* answer = calloc(1, sizeof(struct Resources_Lenght_Pair));
    answer->resources = calloc(1, sizeof(char*));
    answer->lenght = 0;

    char* page = calloc (4096, sizeof(char));
    size_t len = 0;
    // ReSharper disable once CppEntityAssignedButNoRead
    ssize_t read;
    // ReSharper disable once CppDFAUnusedValue
    int pattern_index;
    while (fgets(page, 4096, file) != NULL && !feof(file))
    {
        for(int i = 0; i < NUMBER_OF_PATTERNS; i++)
        {
            int position_after_pattern = 0;
            do {
                const char* line_after_pattern = page + position_after_pattern;
                 pattern_index = BoyerMooreSearch(line_after_pattern, searching_patterns[i]);
                if(pattern_index == -1)
                    continue;

                position_after_pattern = strstr(line_after_pattern, searching_patterns[i]) - page + strlen(searching_patterns[i]);
                char* resource = GetResource(page, line_after_pattern + position_after_pattern);

                if(!IsResourceValid(resource))
                {
                    free(resource);
                    continue;
                }

                answer->lenght++;
                char** new_resources = calloc(answer->lenght, sizeof(char*));
                for(int j = 0; j < answer->lenght - 1; j++)
                {
                    new_resources[j] = answer->resources[j];
                }
                free(answer->resources);
                answer->resources = new_resources;
                answer->resources[answer->lenght - 1] = resource;
            }while(pattern_index != -1);

        }
    }

    fclose(file);
    free(page);
    return answer;
}

int ResourceBeginsWithSlash(const char* resource) {
    return resource[0] == '/';
}

void AddResourcesToUrlTable(const struct UrlTable* url_table, const struct Folder_Resource_Pair* folder_resource_pair, const char* url)
{
    if(strstr(folder_resource_pair->resource, ".css") == NULL &&
       strstr(folder_resource_pair->resource, ".js") == NULL &&
       strstr(folder_resource_pair->resource, ".html") == NULL &&
       strstr(folder_resource_pair->resource, ".php") == NULL)
    {
        return;
    }
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
        free(resource);


        if(new_url == NULL || strlen(new_url) == 0 || !IsUrl(new_url))
        {
            if(new_url != NULL)
                free(new_url);
            continue;
        }

        const int depth = GetUrlDepth(new_url);
        AddUrlToTable(url_table, new_url, depth);

        free(new_url);
    }

    free(root_url);
    free(url_without_resource);
    free(resources_lenght_pair);
}