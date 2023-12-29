//
// Created by cristian on 12.12.2023.
//

#include "file_hierarchy_creator.h"

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "../../../../custom_libraries/custom_c_logger/custom_c_logger.h"
#include "../../../../custom_libraries/custom_c_string/custom_c_string.h"
#include "../url_handler/url_handler.h"

void CreateFolder(const char* path, const char* folder_name)
{
    char* new_path = CombineStrings(3, strlen(path) + strlen(folder_name) + 2, path, "/", folder_name);
    DIR* dir = opendir(new_path);
    if(dir)
    {
        free(new_path);
        closedir(dir);
        return;
    }
    if(mkdir(new_path, 0777) == -1)
    {
        const char* pattern = "Folder '%s' could not be created. It is possible that it already exists.";
        char* message = calloc(strlen(pattern) + strlen(new_path) + 10, sizeof(char));
        sprintf(message, pattern, new_path);
        LogWarning(message);
        free(message);
        free(new_path);
    }
}

char* WalkFromStartToResourceLocation(const char* start_location, const char* url, const int with_create) {
    char* url_without_prefix = RemoveUrlPrefix(url);
    char* url_without_last_slash = RemoveLastSlash(url_without_prefix);
    free(url_without_prefix);
    const struct Tokens_Length_Pair* tokens_length_pair = ExtractUrlTokens(url_without_last_slash);
    free(url_without_last_slash);

    char* path = DuplicateString(start_location);

    for(int i = 0; i < tokens_length_pair->length-1; i++)
    {
        if(with_create > 0)
            CreateFolder(path, tokens_length_pair->tokens[i]);

        char* new_path = CombineStrings(3, strlen(path) + strlen(tokens_length_pair->tokens[i]) + 2, path, "/", tokens_length_pair->tokens[i]);
        free(path);
        path = new_path;
    }

    const char* token = tokens_length_pair->tokens[tokens_length_pair->length-1];
    if(!TokenHasExtension(token))
    {
        if(with_create > 0)
            CreateFolder(path, token);

        char* new_path = CombineStrings(3, strlen(path) + strlen(token) + 2, path, "/", token);
        free(path);
        path = new_path;
    }

    FreeTokensLengthPair(tokens_length_pair);

    return path;
}

void CreateHierarchyFromUrl(const char* starting_location, const char* url)
{
    char* folder_path = WalkFromStartToResourceLocation(starting_location, url, 1);
    free(folder_path);
}


struct Folder_Resource_Pair* GetPathToResource(const char* starting_location, const char* url) {

    struct Folder_Resource_Pair* folder_resource_pair = calloc(1, sizeof(struct Folder_Resource_Pair));
    folder_resource_pair->folder = WalkFromStartToResourceLocation(starting_location, url, 0);

    const struct Tokens_Length_Pair* tokens_length_pair = ExtractUrlTokens(url);
    const char* token = tokens_length_pair->tokens[tokens_length_pair->length-1];

    if(!TokenHasExtension(token))
    {
        folder_resource_pair->resource = strdup("index.html");
    }
    else
    {
        folder_resource_pair->resource = strdup(token);
    }

    FreeTokensLengthPair(tokens_length_pair);

    return folder_resource_pair;
}

void FreeFolderResourcePair(struct Folder_Resource_Pair* folder_resource_pair) {
    free(folder_resource_pair->folder);
    free(folder_resource_pair->resource);
    free(folder_resource_pair);
}
