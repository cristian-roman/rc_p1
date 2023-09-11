//
// Created by root on 9/11/23.
//

#ifndef SERVER_SERVER_NETWORK_H
#define SERVER_SERVER_NETWORK_H

typedef void (*LogMessageFunctionPtr)(const char* message);
typedef void (*LogMessageFromPatternFunctionPtr)(const char* message, ...);

#define PORT 5050
#define MAX_CLIENTS 5

struct ILogger
{
    LogMessageFunctionPtr LogInfo;
    LogMessageFromPatternFunctionPtr LogInfoFromPattern;

    LogMessageFunctionPtr LogError;
    LogMessageFromPatternFunctionPtr LogErrorFromPattern;

} NETWORK_LOGGER;

int SERVER_SOCKET;

void InitLogger( LogMessageFunctionPtr LogInfo,
                 LogMessageFromPatternFunctionPtr LogInfoFromPattern,
                 LogMessageFunctionPtr LogError,
                 LogMessageFromPatternFunctionPtr LogErrorFromPattern);

void CreateServerSocket();

void BindServerSocket();

void OpenPortForListening();

void InitServerSideNetwork(LogMessageFunctionPtr logInfo,
                           LogMessageFromPatternFunctionPtr logInfoFromPattern,
                           LogMessageFunctionPtr logError,
                           LogMessageFromPatternFunctionPtr logErrorFromPattern);

#endif //SERVER_SERVER_NETWORK_H
