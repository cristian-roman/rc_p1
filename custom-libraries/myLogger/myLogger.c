//
// Created by cristian.roman on 9/9/23.
//

#include "myLogger.h"
#include <dirent.h>
#include <sys/stat.h>
#include <malloc.h>
#include <stdarg.h>
#include <string.h>
#include "../system_info/system_info.h"

void LogInfoFromPattern(const char* pattern, ...)
{
    char* logMessage = (char*) malloc(MAX_LOG_MESSAGE_ALLOCATION_SIZE);
    sprintf(logMessage, "");

    va_list args;
    va_start(args, pattern);
    vsnprintf(logMessage, MAX_LOG_MESSAGE_ALLOCATION_SIZE, pattern, args);
    va_end(args);

    LogInfo(logMessage);
    free(logMessage);
}

void LogErrorFromPattern(const char* pattern, ...)
{
    char* logMessage = (char*) malloc(MAX_LOG_MESSAGE_ALLOCATION_SIZE);
    sprintf(logMessage, "");

    va_list args;
    va_start(args, pattern);
    vsnprintf(logMessage, MAX_LOG_MESSAGE_ALLOCATION_SIZE, pattern, args);
    va_end(args);

    LogInfo(logMessage);
    free(logMessage);
}

void LogMessageFromPattern(const char* logLevel, const char* pattern, ...)
{
    char* logMessage = (char*) malloc(MAX_LOG_MESSAGE_ALLOCATION_SIZE);
    sprintf(logMessage, "");

    va_list args;
    va_start(args, pattern);
    vsnprintf(logMessage, MAX_LOG_MESSAGE_ALLOCATION_SIZE, pattern, args);
    va_end(args);

    LogMessage(logLevel, logMessage);
    free(logMessage);
}

void LogInfoByConcat(int numStrings, ...)
{
    char* logMessage = (char*) malloc(MAX_LOG_MESSAGE_ALLOCATION_SIZE);
    sprintf(logMessage, "");

    va_list args;
    va_start(args, numStrings);
    for(int i = 0; i < numStrings; i++) {
        const char* str = va_arg(args, const char*);
        strcat(logMessage, str);
    }

    va_end(args);

    LogInfo(logMessage);
    free(logMessage);
}

void LogErrorByConcat(int numStrings, ...)
{
    char* logMessage = (char*) malloc(MAX_LOG_MESSAGE_ALLOCATION_SIZE);
    sprintf(logMessage, "");

    va_list args;
    va_start(args, numStrings);
    for(int i = 0; i < numStrings; i++) {
        const char* str = va_arg(args, const char*);
        strcat(logMessage, str);
    }

    va_end(args);

    LogError(logMessage);
    free(logMessage);
}


void LogMessageByConcat(const char* logLevel, int numStrings, ...)
{
    char* logMessage = (char*) malloc(MAX_LOG_MESSAGE_ALLOCATION_SIZE);
    sprintf(logMessage, "");

    va_list args;
    va_start(args, numStrings);
    for(int i = 0; i < numStrings; i++) {
        const char* str = va_arg(args, const char*);
        strcat(logMessage, str);
    }

    va_end(args);

    LogMessage(logLevel, logMessage);
    free(logMessage);
}

void LogInfo(const char* message)
{
    LogMessage(INFO_LOG_LEVEL, message);
}

void LogError(const char* message)
{
    LogMessage(ERROR_LOG_LEVEL, message);
}

void LogMessage(const char* logLevel, const char* logMessage)
{
    FILE *fp;
    fp = fopen(CURRENT_LOG_FILE_PATH, "a+");

    char * buildTime = (char*) malloc(CURRENT_BUILD_TIME_SIZE);
    GetCurrentBuildTime(buildTime);

    fprintf(fp, "[%s] [%s]:\n%s\n", buildTime, logLevel, logMessage);
    free(buildTime);

    fclose(fp);
}

void Set_TODAY_LOG_FOLDER_PATH() {
    DIR *dir = opendir(LOG_FOLDER_PATH);
    if (dir) {
        closedir(dir);
    } else {
        mkdir(LOG_FOLDER_PATH, 0777);
    }

    char* todayDate = (char*) malloc(CURRENT_DATE_STRING_SIZE);
    GetCurrentDate(todayDate);

    TODAY_LOG_FOLDER_PATH = malloc(TODAY_LOG_FOLDER_PATH_SIZE);
    sprintf(TODAY_LOG_FOLDER_PATH, "%s/%s", LOG_FOLDER_PATH, todayDate);

    free(todayDate);

    dir = opendir(TODAY_LOG_FOLDER_PATH);
    if (dir) {
        closedir(dir);
    } else {
        mkdir(TODAY_LOG_FOLDER_PATH, 0777);
    }
}

void Set_CURRENT_LOG_FILE_NAME()
{
    char * buildTime = (char*) malloc(CURRENT_BUILD_TIME_SIZE);
    GetCurrentBuildTime(buildTime);

    CURRENT_LOG_FILE_NAME = malloc(CURRENT_LOG_FILE_NAME_SIZE);
    sprintf(CURRENT_LOG_FILE_NAME, "log_%s.txt", buildTime);
    free(buildTime);
}

void Set_CURRENT_LOG_FILE_PATH()
{
    CURRENT_LOG_FILE_PATH = malloc(CURRENT_LOG_FILE_PATH_SIZE);
    sprintf(CURRENT_LOG_FILE_PATH, "%s/%s", TODAY_LOG_FOLDER_PATH, CURRENT_LOG_FILE_NAME);
}

void InitMyLogger()
{
    Set_TODAY_LOG_FOLDER_PATH();
    Set_CURRENT_LOG_FILE_NAME();
    Set_CURRENT_LOG_FILE_PATH();

    LogInfo("'myLogger' library has been initialized");
}