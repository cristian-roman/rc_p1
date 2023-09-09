//
// Created by root on 9/9/23.
//

#ifndef SERVER_MYLOGGER_H
#define SERVER_MYLOGGER_H

#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <malloc.h>

#define LOG_FOLDER_PATH "./logs"

char* TODAY_LOG_FOLDER_PATH;

char* CURRENT_LOG_FILE_NAME;
char* CURRENT_LOG_FILE_PATH;

const char* GetCurrentDate();
const char* GetCurrentBuildTime();

void Set_TODAY_LOG_FOLDER_PATH(); //if it does not exist
void Set_CURRENT_LOG_FILE_NAME();
void Set_CURRENT_LOG_FILE_PATH();

void InitMyLogger();

#endif //SERVER_MYLOGGER_H
