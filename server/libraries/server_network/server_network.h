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

#define MAX_LOG_MESSAGE_ALLOCATION_SIZE 65536
struct ILogger
{
    LogMessageFunctionPtr LogInfo;
    LogMessageFunctionPtr LogError;

} NETWORK_LOGGER;

enum OperationStatus
{
    FAILED,
    SUCCEEDED
};

enum OperationStatus NETWORK_OPERATION_STATUS;

int SERVER_SOCKET;

void InitLogger( LogMessageFunctionPtr LogInfo,
                 LogMessageFunctionPtr LogError);

void CreateServerSocket();

void BindServerSocket();

void OpenPortForListening();

int WaitForClients();

void InitServerSideNetwork(LogMessageFunctionPtr logInfo,
                           LogMessageFunctionPtr logError);

#endif //SERVER_SERVER_NETWORK_H
