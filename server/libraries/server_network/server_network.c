//
// Created by root on 9/11/23.
//

#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include "server_network.h"

void InitServerSideNetwork(LogMessageFunctionPtr logInfo,
                           LogMessageFromPatternFunctionPtr logInfoFromPattern,
                           LogMessageFunctionPtr logError,
                           LogMessageFromPatternFunctionPtr logErrorFromPattern)
{
    InitLogger(logInfo, logInfoFromPattern, logError, logErrorFromPattern);

    CreateServerSocket();

    BindServerSocket();

    OpenPortForListening();
}

void OpenPortForListening()
{
    if (listen(SERVER_SOCKET, MAX_CLIENTS) == -1) {
        NETWORK_LOGGER.LogError("Server application shutting down.... [REASON] socket opening for listen failed");
        exit(EXIT_FAILURE);
    }

    NETWORK_LOGGER.LogInfoFromPattern("Server listening on port %d...", PORT);
}

void BindServerSocket()
{
    struct sockaddr_in server_addr;

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if(bind(SERVER_SOCKET, (struct sockaddr*)&server_addr, sizeof(server_addr)) ==1)
    {
        NETWORK_LOGGER.LogError("Server application shutting down.... [REASON] socket binding failed");
        exit(EXIT_FAILURE);
    }
}

void CreateServerSocket()
{
    SERVER_SOCKET = socket(AF_INET, SOCK_STREAM, 0);
    if(SERVER_SOCKET == -1)
    {
        NETWORK_LOGGER.LogError("Server application shutting down.... [REASON] socket creation failed");
        exit(EXIT_FAILURE);
    }
}

void InitLogger( LogMessageFunctionPtr logInfo,
                 LogMessageFromPatternFunctionPtr logInfoFromPattern,
                 LogMessageFunctionPtr logError,
                 LogMessageFromPatternFunctionPtr logErrorFromPattern)
{
    NETWORK_LOGGER.LogInfo = logInfo;
    NETWORK_LOGGER.LogInfoFromPattern = logInfoFromPattern;

    NETWORK_LOGGER.LogError = logError;
    NETWORK_LOGGER.LogErrorFromPattern = logErrorFromPattern;
}