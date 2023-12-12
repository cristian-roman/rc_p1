//
// Created by cristian.roman on 9/9/23.
//

#ifndef SERVER_MYLOGGER_H
#define SERVER_MYLOGGER_H

#define INFO_LOG_LEVEL "INFO"
#define WARNING_LOG_LEVEL "WARNING"
#define ERROR_LOG_LEVEL "ERROR"

#define GREEN_COLOR "\033[0;32m"
#define BOLD_GREEN_COLOR "\033[1;32m"

#define YELLOW_COLOR "\033[0;33m"
#define BOLD_YELLOW_COLOR "\033[1;33m"

#define RED_COLOR "\033[0;31m"
#define BOLD_RED_COLOR "\033[1;31m"

#define RESET_COLOR "\033[0m"

#define STDOUT "stdout"
extern short STDOUT_FD;

extern short total_number_of_locations;
extern char** log_file_paths;

void LogMessage(const char* log_level, const char* build_time, const char* log_message);

void LogInfo(const char* message);
void LogWarning(const char* message);
void LogError(const char* message);

void InitMyLogger(short number_of_locations, ...);

#endif  // SERVER_MYLOGGER_H
