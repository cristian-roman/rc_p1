#include "web_handler.h"
#include "../../../custom_libraries/custom_c_logger/custom_c_logger.h"
#include "../../../custom_libraries/custom_c_string/custom_c_string.h"
#include <stdlib.h>
#include <string.h>

#include "downloader/downloader.h"
#include "file_hierarchy_creator/file_hierarchy_creator.h"
#include "url_extractor/url_extractor.h"

#define DOWNLOAD_LOCATION "downloaded_resources"

char* DownloadResource(char* url) {

    LogInfo("Creating hierarchy for downloaded resources");
    CreateFolder(".", DOWNLOAD_LOCATION);

    char* starting_path = CombineStrings(3, strlen(DOWNLOAD_LOCATION) + 2, ".", "/", DOWNLOAD_LOCATION);
    CreateHierarchyFromUrl(starting_path, url);
    LogInfo("Hierarchy created successfully");
    char* path_to_resource = GetPathToResource(starting_path, url);
    free(starting_path);

    DownloadOneResource(url, path_to_resource);
    return path_to_resource;
}

void DumpUrl(char* url, const int depth)
{
    if(depth == 0)
    {
        LogInfo("Depth limit reached. Download finished");
        return;
    }

    char* path_to_resource = DownloadResource(url);

    int number_of_resources;
    char** resources_names = ExtractResourcesNames(path_to_resource, &number_of_resources);

    int refferenced_urls_count;
    char** refferenced_urls = ExtractReferencedURLs(resources_names, number_of_resources, url, &refferenced_urls_count);

    for(int i = 0; i < refferenced_urls_count; i++)
    {
        const char* pattern = "Dumping url: %s";
        char* message = GetStringFromPattern(pattern, strlen(pattern) + strlen(refferenced_urls[i]) + 10, refferenced_urls[i]);
        LogWarning(message);
        free(message);

        char* path = DownloadResource(refferenced_urls[i]);
        free(path);
    }

    int on_level_resources_count;
    char** on_level_resources = ExtractOnLevelURLs(resources_names, number_of_resources, url, &on_level_resources_count);

    for(int i = 0; i < on_level_resources_count; i++)
    {
        const char* pattern = "Dumping url: %s";
        char* message = GetStringFromPattern(pattern, strlen(pattern) + strlen(on_level_resources[i]) + 10, on_level_resources[i]);
        LogWarning(message);
        free(message);

        char* path = DownloadResource(on_level_resources[i]);
        free(path);
    }

    FreeResources(on_level_resources, on_level_resources_count);
    FreeResources(refferenced_urls, refferenced_urls_count);
    FreeResources(resources_names, number_of_resources);
    free(path_to_resource);
}