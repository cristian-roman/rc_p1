//
// Created by cristian-roman on 9/7/23.
//

#include <stdio.h>
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


int main() {

    CreateLogLocation();
    CreateCurrentLogFile();

    FILE *fp;
    fp = fopen(CURRENT_LOG_FILE_PATH, "w+");
    fprintf(fp, "BUILD STARTED AT: %s\n", GetCurrentBuildTime());
}

void CreateCurrentLogFile()
{
    const char * buildTime = GetCurrentBuildTime();
    CURRENT_LOG_FILE_NAME = malloc(100);
    sprintf(CURRENT_LOG_FILE_NAME, "log_%s.txt", buildTime);

    CURRENT_LOG_FILE_PATH = malloc(100);
    sprintf(CURRENT_LOG_FILE_PATH, "%s/%s", TODAY_LOG_FOLDER_PATH, CURRENT_LOG_FILE_NAME);
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

void CreateLogLocation() {
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

const char* GetCurrentDate() {

    time_t t;
    struct tm *tm_info;

    time(&t);
    tm_info = localtime(&t);

    char * date = malloc(11);
    strftime(date, 11, "%Y-%m-%d", tm_info);

    return date;
}