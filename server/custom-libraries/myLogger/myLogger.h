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
char* CURRENT_LOG_FILE_PATH;


char* TODAY_LOG_FOLDER_PATH;
char* CURRENT_LOG_FILE_NAME;

const char* GetCurrentDate();
void CreateLogLocation(); //if it does not exist

const char* GetCurrentBuildTime();
void CreateCurrentLogFile();

#endif //SERVER_MYLOGGER_H
