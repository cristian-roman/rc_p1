#include "web_handler.h"
#include "../../../custom_libraries/custom_c_logger/custom_c_logger.h"
#include "../../../custom_libraries/custom_c_string/custom_c_string.h"
#include <stdlib.h>
#include <string.h>

#include "downloader/downloader.h"
#include "file_hierarchy_creator/file_hierarchy_creator.h"
#include "url_extractor/url_extractor.h"

#define DOWNLOAD_LOCATION "downloaded_resources"

struct Folder_Resource_Pair* DownloadResource(char* url) {

    LogInfo("Creating hierarchy for downloaded resources");

    CreateFolder(".", DOWNLOAD_LOCATION);
    char* start_path = CombineStrings(3, strlen(DOWNLOAD_LOCATION) + 2, ".", "/", DOWNLOAD_LOCATION);
    CreateHierarchyFromUrl(start_path, url);
    struct Folder_Resource_Pair* folder_resource_pair = GetPathToResource(start_path, url);
    free(start_path);

    LogInfo("Hierarchy created successfully");

    char* path_to_resource = CombineStrings(3, strlen(folder_resource_pair->folder) + strlen(folder_resource_pair->resource) + 2,
                                                folder_resource_pair->folder, "/", folder_resource_pair->resource);
    DownloadOneResource(url, path_to_resource);
    free(path_to_resource);

    return folder_resource_pair;
}

void DumpUrlTable(struct UrlTable* url_table, const int current_depth, const int max_depth)
{
    if(current_depth > max_depth)
    {
        LogInfo("Depth limit reached. Finishing dumping...");
        return;
    }

    for(url_table->read_url_count[current_depth] = 0;
        url_table->read_url_count[current_depth]< url_table->total_url_count[current_depth];
        url_table->read_url_count[current_depth]+=1)
        {
            const int j = url_table->read_url_count[current_depth];
            char* url = url_table->url_table[current_depth][j];

            struct Folder_Resource_Pair* folder_resource_pair = DownloadResource(url);

            AddResourcesToUrlTable(url_table, folder_resource_pair, url);

            FreeFolderResourcePair(folder_resource_pair);
        }

    DumpUrlTable(url_table, current_depth + 1, max_depth);

    while(url_table->read_url_count[current_depth] < url_table->total_url_count[current_depth]) {
        url_table->read_url_count[current_depth] += 1;
        const int j = url_table->read_url_count[current_depth];
        char* url = url_table->url_table[current_depth][j];

        struct Folder_Resource_Pair* folder_resource_pair = DownloadResource(url);
        FreeFolderResourcePair(folder_resource_pair);
    }
}