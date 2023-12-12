//
// Created by cristian on 12.12.2023.
//

#include "file_hierarchy_creator.h"

#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "../../../../custom_libraries/custom_c_logger/custom_c_logger.h"
#include "../../../../custom_libraries/custom_c_string/custom_c_string.h"

char* RemoveURLPrefix(const char* url)
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

void CreateFolder(const char* path, const char* folder_name)
{
    char* new_path = CombineStrings(3, strlen(path) + strlen(folder_name) + 2, path, "/", folder_name);

    DIR* dir = opendir(new_path);
    if(dir)
    {
        closedir(dir);
        free(new_path);
        return;
    }
    mkdir(new_path, 0777);
    free(new_path);
}

void CreateHierarchyFromUrl(const char* starting_location, const char* url)
{
    char* url_without_prefix = RemoveURLPrefix(url);
    const int number_of_tokens = GetNumberOfTokens(url_without_prefix, '/');
    char** tokens = SplitString(url_without_prefix, '/', number_of_tokens);
    free(url_without_prefix);

    char* path = malloc(strlen(starting_location) + 1);
    strcpy(path, starting_location);

    for(int i = 0; i < number_of_tokens-1; i++)
    {
        if(tokens[i] == NULL || strlen(tokens[i]) == 0)
        {
            continue;
        }
        CreateFolder(path, tokens[i]);
        char* new_path = CombineStrings(3, strlen(path) + strlen(tokens[i]) + 2, path, "/", tokens[i]);
        free(path);
        path = new_path;
    }
    free(path);
    FreeSplitString(tokens, number_of_tokens);
}

char* ExtractResourceFileNameWithExtension(const char* url) {
    const int len = strlen(url);

    for(int i = len-2; i >= 0; i--)
    {
        if(url[i] == '/')
        {
            char* file_name = malloc(len - i);
            strcpy(file_name, url + i + 1);
            return file_name;
        }
    }
    return 0;
}

int FileNameHasExtension(const char* file_name) {
    const int len = strlen(file_name);
    for(int i = 0; i < len; i++)
    {
        if(file_name[i] == '.')
        {
            return 1;
        }
    }
    return 0;
}

char* GetPathToResource(const char* starting_location, const char* url) {

    //serach up to the antepenultimate slash
    char* url_without_prefix = RemoveURLPrefix(url);
    const int number_of_tokens = GetNumberOfTokens(url_without_prefix, '/');
    char** tokens = SplitString(url_without_prefix, '/', number_of_tokens);
    free(url_without_prefix);


    char* path = malloc(strlen(starting_location) + 1);
    strcpy(path, starting_location);

    for(int i = 0; i < number_of_tokens - 1; i++)
    {
        char* new_path = CombineStrings(3, strlen(path) + strlen(tokens[i]) + 2, path, "/", tokens[i]);
        free(path);
        path = new_path;
    }

    FreeSplitString(tokens, number_of_tokens);

    //check if the last token is a file name or a folder name. If it is a folder name, create it and name the resource index.html
    //else just return the path to the resource
    char* file_name = ExtractResourceFileNameWithExtension(url);
    if(!FileNameHasExtension(file_name)) {
        CreateFolder(path, file_name);
        char* new_path = CombineStrings(5, strlen(path) + strlen(file_name) + 2, path, "/", file_name, "/", "index.html");
        free(path);
        path = new_path;
    }
    else {
        char* new_path = CombineStrings(3, strlen(path) + strlen(file_name) + 2, path, "/", file_name);
        free(path);
        path = new_path;
    }
    free(file_name);
    return path;
}
