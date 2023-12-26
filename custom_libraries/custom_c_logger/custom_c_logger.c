#include "custom_c_logger.h"

#include <dirent.h>
#include <malloc.h>
#include <sys/stat.h>
#include <stdarg.h>
#include <string.h>
#include <fcntl.h>
#include <sys/file.h>
#include <errno.h>
#include <unistd.h>

#include "../system_info/system_info.h"

short STDOUT_FD;

short total_number_of_locations;
char** log_file_paths;

void LogInfo(const char* message) {

    char* buildTime = GetCurrentBuildTime();

    LogMessage(INFO_LOG_LEVEL, buildTime, message);

    free(buildTime);
}

void LogWarning(const char* message) {

    char* buildTime = GetCurrentBuildTime();

    LogMessage(WARNING_LOG_LEVEL, buildTime, message);

    free(buildTime);
}

void LogError(const char* message) {

    char* buildTime = GetCurrentBuildTime();

    LogMessage(ERROR_LOG_LEVEL, buildTime, message);

    free(buildTime);
}


void PrintMessage(const char* log_level, const char* build_time, const char* log_message)
{
    if(strcmp(log_level, INFO_LOG_LEVEL) == 0) {
        printf(BOLD_GREEN_COLOR);
        printf("[INFO]\n");
        printf(GREEN_COLOR);
        printf("[%s] %s\n", build_time, log_message);
        printf(RESET_COLOR);
    }
    else if(strcmp(log_level, WARNING_LOG_LEVEL) == 0) {
        printf(BOLD_YELLOW_COLOR);
        printf("[WARNING]\n");
        printf(YELLOW_COLOR);
        printf("[%s] %s\n", build_time, log_message);
        printf(RESET_COLOR);
    }
    else if(strcmp(log_level, ERROR_LOG_LEVEL) == 0) {
        printf(BOLD_RED_COLOR);
        printf("[ERROR]\n");
        printf(RED_COLOR);
        printf("[%s] %s\n", build_time, log_message);
        printf(RESET_COLOR);
    }
    else {
        printf("[%s]", log_level);
        printf("[%s] %s\n", build_time, log_message);
    }
}

int LockFile(const char* file_path, int* lock_fd) {
    *lock_fd = open(file_path, O_RDWR | O_CREAT, 0644);
    if (*lock_fd == -1) {
        return -1;  // Failed to open the file
    }

    // Try to acquire a lock on the file, entering a retry loop if it's already locked
    while (flock(*lock_fd, LOCK_EX | LOCK_NB) == -1) {
        if (errno == EWOULDBLOCK) {
            // Another process holds the lock, retry after a delay
            sleep(1);
        } else {
            // Some other error occurred
            close(*lock_fd);
            return -1;
        }
    }

    return 0;  // Lock acquired successfully
}

void LogMessage(const char* log_level, const char* build_time, const char* log_message) {
    for(int i = 0; i < total_number_of_locations; i++) {
        int lock_fd;
        if (LockFile(log_file_paths[i], &lock_fd) == 0) {
            FILE* fp = fopen(log_file_paths[i], "a+");
            if (fp != NULL) {
                fprintf(fp, "[%s] [%s]:\n%s\n", build_time, log_level, log_message);
                fclose(fp);
            }

            // Release the lock on the file
            flock(lock_fd, LOCK_UN);
            close(lock_fd);
        }
    }

    if(STDOUT_FD != -1) {
        PrintMessage(log_level, build_time, log_message);
    }
}

void AddTodayFolderToPaths() {
    char* today_date = GetCurrentDate();
    for(int i = 0; i < total_number_of_locations; i++) {
        char* current_path = log_file_paths[i];

        DIR* dir = opendir(current_path);
        if (dir) {
            closedir(dir);
        } else {
            mkdir(current_path, 0777);
        }
        
        current_path = realloc(current_path, strlen(current_path) + strlen(today_date) + 2);
        bzero(current_path + strlen(current_path), strlen(today_date) + 2);
        sprintf(current_path, "%s/%s", current_path, today_date);
        log_file_paths[i] = current_path;

        dir = opendir(log_file_paths[i]);
        if (dir) {
            closedir(dir);
        } else {
            mkdir(log_file_paths[i], 0777);
        }
    }
    free(today_date);
}

void AttacheLogFileToPaths() {
    char* build_time = GetCurrentBuildTime();
    for(int i = 0; i < total_number_of_locations; i++) {
        const char* extension = ".log";
        char* current_path = log_file_paths[i];
        current_path = realloc(current_path, strlen(current_path) + strlen(build_time) + strlen(extension) + 2);
        bzero(current_path + strlen(current_path), strlen(build_time) + strlen(extension) + 2);
        sprintf(current_path, "%s/log_%s%s", current_path, build_time, extension);
        log_file_paths[i] = current_path;

        FILE* fp = fopen(log_file_paths[i], "a+");
        fclose(fp);
    }
    free(build_time);
}

void InitMyLogger(const short number_of_locations, ...) {
    STDOUT_FD = -1;
    total_number_of_locations = number_of_locations;
    log_file_paths = malloc(number_of_locations * sizeof(char*));
    va_list args;
    va_start(args, number_of_locations);

    for (int i = 0; i < number_of_locations; i++) {
        const char* location = va_arg(args, const char*);
        if (strcmp(location, STDOUT) == 0) {
            STDOUT_FD = 1;
            total_number_of_locations--;
        } else {
            log_file_paths[i] = malloc(strlen(location) + 1);
            strcpy(log_file_paths[i], location);
        }
    }
    va_end(args);

    AddTodayFolderToPaths();
    AttacheLogFileToPaths();

    LogInfo("'custom_c_logger' library has been initialized");
}