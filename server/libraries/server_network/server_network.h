//
// Created by root on 9/11/23.
//

#ifndef SERVER_SERVER_NETWORK_H
#define SERVER_SERVER_NETWORK_H

#include <stdbool.h>

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

enum OperationStatus
{
    FAILED,
    SUCCEEDED
};

enum OperationStatus NETWORK_OPERATION_STATUS;

int SERVER_SOCKET;

void InitLogger( LogMessageFunctionPtr LogInfo,
                 LogMessageFromPatternFunctionPtr LogInfoFromPattern,
                 LogMessageFunctionPtr LogError,
                 LogMessageFromPatternFunctionPtr LogErrorFromPattern);

void CreateServerSocket();

void BindServerSocket();

void OpenPortForListening();

int WaitForClients();

void InitServerSideNetwork(LogMessageFunctionPtr logInfo,
                           LogMessageFromPatternFunctionPtr logInfoFromPattern,
                           LogMessageFunctionPtr logError,
                           LogMessageFromPatternFunctionPtr logErrorFromPattern);

#endif //SERVER_SERVER_NETWORK_H
