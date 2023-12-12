//
// Created by cristian on 12.12.2023.
//

#ifndef URL_EXTRACTOR_H
#define URL_EXTRACTOR_H

#define SOURCE_LEVEL_URLS 0
#define ON_LEVEL_URLS 1
#define NEXT_LEVEL_URLS 2

char** ExtractResourcesNames(const char* resource_path, int *number_of_resources_found);
void FreeResources(char** resources_names, const int number_of_resources_found);
char** ExtractReferencedURLs(const char** resources_names, const int number_of_resources_found , const char* url, int* refferenced_urls_count);
// char** ExtractOnLevelURLs(const char** resourcesNames, const char* url);
// char** ExtractNextLevelURLs(const char** resourcesNames, const char* url);
#endif //URL_EXTRACTOR_H
