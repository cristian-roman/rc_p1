//
// Created by cristian on 12.12.2023.
//

#include "downloader.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <curl/curl.h>

#include "../../../../custom_libraries/custom_c_logger/custom_c_logger.h"
#include "../../../../custom_libraries/custom_c_string/custom_c_string.h"


static size_t write_data(const void *ptr, const size_t size, const size_t nmemb, FILE *stream) {
    return fwrite(ptr, size, nmemb, stream);
}

void DownloadOneResource(char* url, const char* path_to_resource) {
    FILE* file = fopen(path_to_resource, "wb");
    if(file == NULL)
    {
        LogError("Failed to open file to save the document. Someone else might be trying to download the same page at the same time. Download interrupted");
        return;
    }

    fseek(file, 0, SEEK_END);
    const long current_pos = ftell(file);

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

    curl_easy_setopt(curl, CURLOPT_URL, url);

    if (current_pos > 0) {
        curl_easy_setopt(curl, CURLOPT_RESUME_FROM_LARGE, current_pos);
    }

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);

    LogInfo("Downloading started...");
    CURLcode res = curl_easy_perform(curl);

    int sleep_time = 5;
    for (int tries = 0; tries < 3 && res == CURLE_PARTIAL_FILE; tries++) {
        pattern = "Failed to download the page. Retrying in %d seconds... (Try %d/3)";
        message = GetStringFromPattern(pattern, strlen(pattern) + 10, sleep_time, tries + 1);
        LogWarning(message);
        free(message);

        sleep(sleep_time);
        long file_size;
        curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD_T, &file_size);
        curl_easy_setopt(curl, CURLOPT_RESUME_FROM_LARGE, file_size);
        res = curl_easy_perform(curl);
        sleep_time *= 2;
    }


    if(res!=CURLE_OK)
    {
        pattern = "Failed to download the page. Error code: %d";
        message = GetStringFromPattern(pattern, strlen(pattern) + 10, res);
        LogError(message);
        free(message);
        fclose(file);
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        return;
    }

    LogInfo("Downloading finished successfully");
    fflush(file);
    curl_easy_cleanup(curl);
    fclose(file);
    curl_global_cleanup();
}
