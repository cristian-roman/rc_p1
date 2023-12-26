#include "system_info.h"

#include <stdlib.h>
#include <strings.h>
#include <time.h>

char* GetCurrentBuildTime()
{
    time_t t;
    time(&t);
    const struct tm* tm_info = localtime(&t);

    const short CURRENT_BUILD_TIME_SIZE = 20;
    char* buildTime = malloc(CURRENT_BUILD_TIME_SIZE);
    bzero(buildTime, CURRENT_BUILD_TIME_SIZE);
    strftime(buildTime, CURRENT_BUILD_TIME_SIZE, "%H:%M:%S", tm_info);

    return buildTime;
}

char* GetCurrentDate() {

    time_t t;
    time(&t);
    const struct tm* tm_info = localtime(&t);

    const short CURRENT_DATE_STRING_SIZE = 20;
    char* currentDate = malloc(CURRENT_DATE_STRING_SIZE);
    bzero(currentDate, CURRENT_DATE_STRING_SIZE);
    strftime(currentDate, CURRENT_DATE_STRING_SIZE, "%Y-%m-%d", tm_info);

    return currentDate;
}