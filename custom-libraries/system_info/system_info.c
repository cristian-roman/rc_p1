

#include <time.h>

char * GetCurrentBuildTime(char* buildTime)
{
    time_t t;

    time(&t);
    const struct tm* tm_info = localtime(&t);

    strftime(buildTime, 11, "%H:%M:%S", tm_info);

    return buildTime;
}

char* GetCurrentDate(char* currentDate) {

    time_t t;

    time(&t);
    const struct tm* tm_info = localtime(&t);

    strftime(currentDate, 11, "%Y-%m-%d", tm_info);

    return currentDate;
}