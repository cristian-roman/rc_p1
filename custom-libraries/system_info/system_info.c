//
// Created by cristian.roman on 9/9/23.
//

#include <time.h>

char * GetCurrentBuildTime(char* buildTime)
{
    time_t t;
    struct tm *tm_info;

    time(&t);
    tm_info = localtime(&t);

    strftime(buildTime, 11, "%H:%M:%S", tm_info);

    return buildTime;
}

char* GetCurrentDate(char* currentDate) {

    time_t t;
    struct tm *tm_info;

    time(&t);
    tm_info = localtime(&t);

    strftime(currentDate, 11, "%Y-%m-%d", tm_info);

    return currentDate;
}