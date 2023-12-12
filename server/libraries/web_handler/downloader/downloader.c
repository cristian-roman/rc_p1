//
// Created by cristian on 12.12.2023.
//

#include "downloader.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../../../../custom_libraries/custom_c_logger/custom_c_logger.h"
#include "../../../../custom_libraries/custom_c_string/custom_c_string.h"


static size_t write_data(const void *ptr, const size_t size, const size_t nmemb, FILE *stream) {
    return fwrite(ptr, size, nmemb, stream);
}

void DownloadOneResource(CURL* curl, char* url, const char* path_to_resource) {
    FILE* file = fopen(path_to_resource, "wb");
    if(file == NULL)
    {
        LogError("Failed to open file to save the document. Someone else might be trying to download the same page at the same time. Download interrupted");
        return;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url);

    // Set the callback function to write data to file
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);

    // Set the file pointer as the write data parameter
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);

    // Set option to resume the download if possible
    curl_easy_setopt(curl, CURLOPT_RESUME_FROM, 0L);

    // Perform the download
    LogInfo("Downloading started...");
    CURLcode res = curl_easy_perform(curl);

    int sleep_time = 5;
    for(int tries = 0; tries < 3 && res == CURLE_PARTIAL_FILE; tries++)
    {
        const char* pattern = "Failed to download the page. Retrying in %d seconds... (Try %d/3)";
        char* message = GetStringFromPattern(pattern, strlen(pattern) + 10, sleep_time, tries + 1);
        LogWarning(message);
        free(message);

        sleep(sleep_time);
        long file_size;
        curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD_T, &file_size);
        curl_easy_setopt(curl, CURLOPT_RESUME_FROM, file_size);
        res = curl_easy_perform(curl);
        sleep_time *= 2;
    }

    if(res!=CURLE_OK)
    {
        const char* pattern = "Failed to download the page. Error code: %d";
        char* message = GetStringFromPattern(pattern, strlen(pattern) + 10, res);
        LogError(message);
        free(message);
        fclose(file);
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        return;
    }

    LogInfo("Downloading finished successfully");

    curl_easy_cleanup(curl);
    fclose(file);
    curl_global_cleanup();
}
