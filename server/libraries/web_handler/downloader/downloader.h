//
// Created by cristian on 12.12.2023.
//

#ifndef DOWNLOADER_H
#define DOWNLOADER_H
#include <curl/curl.h>

void DownloadOneResource(CURL* curl, char* url, const char* path_to_resource);

#endif //DOWNLOADER_H
