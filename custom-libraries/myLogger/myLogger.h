//
// Created by cristian.roman on 9/9/23.
//

#ifndef SERVER_MYLOGGER_H
#define SERVER_MYLOGGER_H

#define LOG_FOLDER_PATH "./logs"

#define INFO_LOG_LEVEL "INFO"
#define WARNING_LOG_LEVEL "WARNING"
#define ERROR_LOG_LEVEL "ERROR"

#define MAX_LOG_MESSAGE_ALLOCATION_SIZE 512

#define TODAY_LOG_FOLDER_PATH_SIZE 25
char* TODAY_LOG_FOLDER_PATH;

#define CURRENT_LOG_FILE_NAME_SIZE 20
char* CURRENT_LOG_FILE_NAME;

#define CURRENT_LOG_FILE_PATH_SIZE 50
char* CURRENT_LOG_FILE_PATH;

void PrintInfo(const char* message);
void PrintInfoFromPattern(const char* message, const char* pattern, ...);

void PrintWarrning(const char* message);
void PrintWarrningFromPattern(const char* message, const char* pattern, ...);

void LogMessage(const char* logLevel, const char* logMessage);
void LogMessageByConcat(const char* logLevel, int numStrings, ...);
void LogMessageFromPattern(const char* logLevel, const char* pattern, ...);

void LogInfo(const char* message);
void LogInfoByConcat(int numStrings, ...);
void LogInfoFromPattern(const char* pattern, ...);

void LogError(const char* message);
void LogErrorByConcat(int numStrings, ...);
void LogErrorFromPattern(const char* pattern, ...);

void InitMyLogger();

#endif  // SERVER_MYLOGGER_H
