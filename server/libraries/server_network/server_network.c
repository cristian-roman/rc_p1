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
        NETWORK_OPERATION_STATUS = FAILED;
    }

    NETWORK_LOGGER.LogInfoFromPattern("Server listening on port %d...", PORT);
    NETWORK_OPERATION_STATUS = SUCCEEDED;
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
        NETWORK_OPERATION_STATUS = FAILED;
    }
    else
    {
        NETWORK_LOGGER.LogInfoFromPattern("Server application has successfully bound the socket:\n- Accepts IpV4 addresses\n- Running at port: %s\n- Accepts requests coming from any ip address"
                                          , PORT);
        NETWORK_OPERATION_STATUS = SUCCEEDED;

    }
}

void CreateServerSocket()
{
    SERVER_SOCKET = socket(AF_INET, SOCK_STREAM, 0);
    if(SERVER_SOCKET == -1)
    {
        NETWORK_LOGGER.LogError("Server application shutting down.... [REASON] socket creation failed");
        NETWORK_OPERATION_STATUS = FAILED;
    }
    else
    {
        NETWORK_LOGGER.LogInfo("Server application has successfully created a socket");
        NETWORK_OPERATION_STATUS = SUCCEEDED;
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

    NETWORK_LOGGER.LogInfo("NETWORK_LOGGER interface implemented by 'myLogger' library");
}

int WaitForClients()
{
    int client_socket;
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);

    client_socket = accept(SERVER_SOCKET, (struct sockaddr*)&client_addr, &addr_len);
    if(client_socket==-1)
    {
        NETWORK_LOGGER.LogError("Server application unable to establish connection with client... [REASON] Client acceptance failed");
        NETWORK_OPERATION_STATUS = FAILED;
    }
    else
    {
        NETWORK_LOGGER.LogInfo("Server application established a connection with a client. Waiting for commands...");
        NETWORK_OPERATION_STATUS = SUCCEEDED;
    }

    return client_socket;
}