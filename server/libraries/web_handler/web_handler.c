#include "web_handler.h"
#include "../../../custom-libraries/myLogger/myLogger.h"
#include "../../../custom-libraries/myString/myString.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <curl/curl.h>
#include <sys/stat.h>
#define DOWNLOAD_LOCATION "downloads"

void create_downloads_folder_if_not_exists()
{
    struct stat st = {0};
    if (stat(DOWNLOAD_LOCATION, &st) == -1)
    {
        LogInfo("Downloads folder does not exist. Creating it...");
        if(mkdir(DOWNLOAD_LOCATION, 0700) == -1)
        {
            LogError("Failed to create downloads folder");
            exit(EXIT_FAILURE);
        }
        LogInfo("Folder created successfully");
    }
    else
    {
        LogInfo("Downloads folder already exists");
    }
}

static size_t write_data(const void *ptr, const size_t size, const size_t nmemb, FILE *stream) {
    return fwrite(ptr, size, nmemb, stream);
}

void download_page(char* url, int url_size, int depth)
{
    LogInfoFromPattern("Downloading page from URL: %s", url);

    curl_global_init(CURL_GLOBAL_DEFAULT);
    CURL* curl = curl_easy_init();
    if(curl == NULL)
    {
        LogError("Failed to initialize curl. Download interrupted");
        curl_global_cleanup();
        return;
    }
    create_downloads_folder_if_not_exists();

    char* encoded_url = curl_easy_escape(curl, url, 0);

    char* file_path = malloc(strlen(DOWNLOAD_LOCATION) + strlen(encoded_url) + 10);
    CombineStrings(file_path, 4, "./", DOWNLOAD_LOCATION, "/", encoded_url);
    LogInfoFromPattern("File path to save page is: %s", file_path);


    //write something to file
    FILE* file = fopen(file_path, "wb");
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
        LogWarningFromPattern("Failed to download the page. Retrying in %d seconds... (Try %d/3)", sleep_time, tries + 1);
        sleep(sleep_time);
        long file_size;
        curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD_T, &file_size);
        curl_easy_setopt(curl, CURLOPT_RESUME_FROM, file_size);
        res = curl_easy_perform(curl);
        sleep_time *= 2;
    }

    if(res!=CURLE_OK)
    {
        LogErrorFromPattern("Failed to download the page. Error code: %d", res);
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