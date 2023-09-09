//
// Created by root on 9/9/23.
//

#include "myLogger.h"

void Set_TODAY_LOG_FOLDER_PATH() {
    DIR *dir = opendir(LOG_FOLDER_PATH);
    if (dir) {
        closedir(dir);
    } else {
        mkdir(LOG_FOLDER_PATH, 0777);
    }

    const char* todayDate = GetCurrentDate();

    TODAY_LOG_FOLDER_PATH = malloc(100);
    sprintf(TODAY_LOG_FOLDER_PATH, "%s/%s", LOG_FOLDER_PATH, todayDate);

    dir = opendir(TODAY_LOG_FOLDER_PATH);
    if (dir) {
        closedir(dir);
    } else {
        mkdir(TODAY_LOG_FOLDER_PATH, 0777);
    }
}

void Set_CURRENT_LOG_FILE_NAME()
{
    const char * buildTime = GetCurrentBuildTime();
    CURRENT_LOG_FILE_NAME = malloc(100);
    sprintf(CURRENT_LOG_FILE_NAME, "log_%s.txt", buildTime);
}

void Set_CURRENT_LOG_FILE_PATH()
{
    CURRENT_LOG_FILE_PATH = malloc(100);
    sprintf(CURRENT_LOG_FILE_PATH, "%s/%s", TODAY_LOG_FOLDER_PATH, CURRENT_LOG_FILE_NAME);
}

void InitMyLogger()
{
    Set_TODAY_LOG_FOLDER_PATH();
    Set_CURRENT_LOG_FILE_NAME();
    Set_CURRENT_LOG_FILE_PATH();
}

const char * GetCurrentBuildTime()
{
    time_t t;
    struct tm *tm_info;

    time(&t);
    tm_info = localtime(&t);

    char * buildTime = malloc(9);
    strftime(buildTime, 11, "%H:%M:%S", tm_info);

    return buildTime;
}

const char* GetCurrentDate() {

    time_t t;
    struct tm *tm_info;

    time(&t);
    tm_info = localtime(&t);

    char * date = malloc(11);
    strftime(date, 11, "%Y-%m-%d", tm_info);

    return date;
}