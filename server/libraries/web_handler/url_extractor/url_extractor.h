//
// Created by cristian on 12.12.2023.
//

#ifndef URL_EXTRACTOR_H
#define URL_EXTRACTOR_H
char** ExtractOnLevelURLs(const char* url, const char* resource_path);
char** ExtractNextLevelURLs(const char* url, const char* resource_path);
#endif //URL_EXTRACTOR_H
