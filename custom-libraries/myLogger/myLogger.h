//
// Created by root on 9/9/23.
//

#ifndef SERVER_MYLOGGER_H
#define SERVER_MYLOGGER_H

#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <malloc.h>
#include <stdarg.h>

#define LOG_FOLDER_PATH "./logs"

#define INFO_LOG_LEVEL "INFO"
#define ERROR_LOG_LEVEL "ERROR"

char* TODAY_LOG_FOLDER_PATH;

char* CURRENT_LOG_FILE_NAME;
char* CURRENT_LOG_FILE_PATH;

const char* GetCurrentDate();
const char* GetCurrentBuildTime();

void Set_TODAY_LOG_FOLDER_PATH(); //if it does not exist
void Set_CURRENT_LOG_FILE_NAME();
void Set_CURRENT_LOG_FILE_PATH();

void LogMessage(const char* logLevel, const char* logMessage);
void LogMessageFromPattern(const char* logLevel, const char* messagePattern, ...);

void LogInfo(const char* message);
void LogInfoFromPattern(const char* messagePattern, ...);

void LogError(const char* message);
void LogErrorFromPattern(const char* messagePattern, ...);

void InitMyLogger();

#endif //SERVER_MYLOGGER_H
