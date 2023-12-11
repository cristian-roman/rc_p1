#include "web_handler.h"
#include "../../../custom-libraries/custom_c_logger/custom_c_logger.h"
#include "../../../custom-libraries/custom_c_string/custom_c_string.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <curl/curl.h>
#include <sys/stat.h>
#define DOWNLOAD_LOCATION "downloads"

char* create_folder_if_not_exists(const char* current_path, char* folder_name)
{
    struct stat st = {0};
    const int total_size = strlen(current_path) + strlen(folder_name) + 2;
    char* path = CombineStrings(3, total_size, current_path, "/", folder_name);

    if (stat(path, &st) == -1)
    {
        LogInfoFromPattern("'%s' folder does not exist at path: %s. Creating it...", folder_name, current_path);
        if(mkdir(path, 0700) == -1)
        {
            LogError("Failed to create downloads folder");
            free(path);
            exit(EXIT_FAILURE);
        }
        LogInfoFromPattern("Folder %s created successfully", folder_name);
        return path;
    }
    LogInfoFromPattern("Foder '%s' already exists at path: %s", folder_name, current_path);
    return path;
}

char* CreatePageHierarchy(const char* url, const char* download_path, const char* root_path) {

    char* url_without_protocol = malloc(strlen(url) + 1);
    strcpy(url_without_protocol, url);
    url_without_protocol = strstr(url_without_protocol, "://") + 3;

    const int number_of_tokens = GetNumberOfTokens(url_without_protocol, '/');
    char** tokens = SplitString(url_without_protocol, '/', number_of_tokens);

    char* path = malloc(strlen(download_path) + 1);
    bzero(path, strlen(download_path) + 1);
    strcpy(path, download_path);

    char* folder_name = tokens[0];
    root_path = create_folder_if_not_exists(path, folder_name);
    free(folder_name);
    strcpy(path, root_path);

    for(int i = 1; i < number_of_tokens; i++)
    {
        folder_name = tokens[i];
        char* new_path = create_folder_if_not_exists(path, folder_name);
        free(folder_name);
        path = new_path;
    }

    return path;
}

char* extractExtension(const char* url) {
    // Find the position of the first '/' after "://"
    const char* pathStart = strstr(url, "://");
    if (pathStart == NULL) {
        LogError("Invalid URL format");
        exit(-1);
    }

    pathStart += 3; // Move past "://"

    // Find the position of the first '/' after the domain
    const char* pathEnd = strrchr(pathStart, '/');
    if (pathEnd == NULL) {
        LogError("Invalid URL format");
        exit(-1);
    }

    // Find the position of the last '.' in the path
    char* extensionStart = strrchr(pathEnd, '.');
    if (extensionStart == NULL || extensionStart < pathEnd) {
        // No '.' found or it's before the last '/'
        return ".html"; // Default extension
    }

    return extensionStart;
}

static size_t write_data(const void *ptr, const size_t size, const size_t nmemb, FILE *stream) {
    return fwrite(ptr, size, nmemb, stream);
}

void download_resource(char* url, int url_size, int depth)
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
    char* download_path = create_folder_if_not_exists(".", DOWNLOAD_LOCATION);
    const char* root_path = NULL;
    char* page_path = CreatePageHierarchy(url, download_path, root_path);
    free(download_path);
    const char* extension = extractExtension(url);
    char* fileNameWithExtension = malloc(20);
    if(strcmp(extension, ".html") == 0)
    {
        strcpy(fileNameWithExtension, "index.html");
    }
    else
    {
        strcpy(fileNameWithExtension, "resource");
        strcat(fileNameWithExtension, extension);
    }
    const int file_path_size = strlen(page_path) + strlen(fileNameWithExtension) + 2;
    char* file_path = CombineStrings(3,file_path_size, page_path, "/", fileNameWithExtension);
    LogInfoFromPattern("Downloading resource at path: %s", file_path);


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