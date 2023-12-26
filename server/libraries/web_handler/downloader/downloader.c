//
// Created by cristian on 12.12.2023.
//

#include "downloader.h"

#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <curl/curl.h>
#include <fcntl.h>
#include <sys/file.h>

#include "../../../../custom_libraries/custom_c_logger/custom_c_logger.h"

#define FreeResources \
    fflush(file); \
    fclose(file); \
    flock(lock_fd, LOCK_UN); \
    close(lock_fd); \
    curl_easy_cleanup(curl); \
    curl_global_cleanup();


static size_t WriteData(const void *ptr, const size_t size, const size_t nmemb, FILE *stream) {
    return fwrite(ptr, size, nmemb, stream);
}

void DownloadOneResource(char* url, const char* path_to_resource) {

    const int lock_fd = open(path_to_resource, O_RDWR | O_CREAT, 0644);
    if (lock_fd == -1) {
        LogError("Failed to open file to save the document. Download interrupted");
        return;
    }

    // Try to acquire a lock on the file, entering a retry loop if it's already locked
    while (flock(lock_fd, LOCK_EX | LOCK_NB) == -1) {
        if (errno == EWOULDBLOCK) {
            // Another process holds the lock, retry after a delay
            LogWarning("Another process holds the lock. Retrying...");
            sleep(5);
        } else {
            // Some other error occurred
            LogError("Failed to acquire lock on the file. Download interrupted");
            close(lock_fd);
            return;
        }
    }

    FILE* file = fopen(path_to_resource, "w");
    if(file == NULL)
    {
        LogError("Failed to open file to save the document. Someone else might be trying to download the same page at the same time. Download interrupted");
        return;
    }

    fseek(file, 0, SEEK_END);
    const long current_pos = ftell(file);

    const char* pattern = "Downloading resources from URL: %s";
    char* message = calloc (strlen(pattern) + strlen(url) + 10, sizeof(char));
    sprintf(message, pattern, url);
    LogInfo(message);
    free(message);

    curl_global_init(CURL_GLOBAL_DEFAULT);
    CURL* curl = curl_easy_init();
    if(curl == NULL)
    {
        LogError("Failed to initialize curl. Download interrupted");
        FreeResources
        return;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url);

    if (current_pos > 0) {
        curl_easy_setopt(curl, CURLOPT_RESUME_FROM_LARGE, current_pos);
    }

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteData);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);

    LogInfo("Downloading started...");
    CURLcode res = curl_easy_perform(curl);

    int sleep_time = 5;
    for (int tries = 0; tries < 3 && res == CURLE_PARTIAL_FILE; tries++) {
        pattern = "Failed to download the page. Retrying in %d seconds... (Try %d/3)";
        message = calloc(strlen(pattern) + 20, sizeof(char));
        sprintf(message, pattern, sleep_time, tries+1);
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
        message = calloc(strlen(pattern) + 10, sizeof(char));
        sprintf(message, pattern, res);
        LogError(message);
        free(message);
        FreeResources
        return;
    }

    FreeResources

    LogInfo("Downloading finished successfully");
}
