#include "web_handler.h"
#include "../../../custom_libraries/custom_c_logger/custom_c_logger.h"
#include "../../../custom_libraries/custom_c_string/custom_c_string.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <curl/curl.h>

#include "downloader/downloader.h"
#include "file_hierarchy_creator/file_hierarchy_creator.h"

#define DOWNLOAD_LOCATION "downloaded_resources"


void DownloadResources(char* url, const int depth)
{
    if(depth == 0)
    {
        LogInfo("Depth limit reached. Download finished");
        return;
    }

    const char* pattern = "Downloading resources from URL: %s";
    char* message = GetStringFromPattern(pattern, strlen(pattern) + strlen(url) + 10, url);
    LogInfo(message);
    free(message);

    curl_global_init(CURL_GLOBAL_DEFAULT);
    CURL* curl = curl_easy_init();
    if(curl == NULL)
    {
        LogError("Failed to initialize curl. Download interrupted");
        curl_global_cleanup();
        return;
    }

    LogInfo("Creating hierarchy for downloaded resources");
    CreateFolder(".", DOWNLOAD_LOCATION);
    char* starting_path = CombineStrings(3, strlen(DOWNLOAD_LOCATION) + 2, ".", "/", DOWNLOAD_LOCATION);
    CreateHierarchyFromUrl(starting_path, url);
    LogInfo("Hierarchy created successfully");
    char* path_to_resource = GetPathToResource(starting_path, url);
    free(starting_path);

    DownloadOneResource(curl, url, path_to_resource);
    free(path_to_resource);
}