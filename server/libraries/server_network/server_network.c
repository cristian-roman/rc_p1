#include "server_network.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <malloc.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>

#include "../../../custom-libraries/myString/myString.h"
#include "../../../custom-libraries/myLogger/myLogger.h"

void InitServerSideNetwork()
{
    CreateServerSocket();

    BindServerSocket();

    OpenPortForListening();
}

void OpenPortForListening()
{
    if (listen(SERVER_SOCKET, MAX_CLIENTS) == -1) {
        LogError("Server application shutting down.... [REASON] socket opening for listen failed");
        NETWORK_OPERATION_STATUS = FAILED;
        exit(-1);
    }

    char* portAsString = malloc(10);
    LogInfoFromPattern("Server listening on port %s...", IntegerToString(portAsString, PORT));
    free(portAsString);

    NETWORK_OPERATION_STATUS = SUCCEEDED;
}

void BindServerSocket()
{
    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if(bind(SERVER_SOCKET, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)
    {
        LogError("Server application shutting down.... [REASON] socket binding failed");
        NETWORK_OPERATION_STATUS = FAILED;
        exit(-1);
    }
    char* portAsString = malloc(10);
    LogInfoFromPattern("Server application has successfully set the socket:\n- Accepts IpV4 addresses\n- Can be found at port: %s\n- Accepts requests coming from any ip address",
                       IntegerToString(portAsString, PORT));
    free(portAsString);

    NETWORK_OPERATION_STATUS = SUCCEEDED;
}

void CreateServerSocket()
{
    SERVER_SOCKET = socket(AF_INET, SOCK_STREAM, 0);
    if(SERVER_SOCKET == -1)
    {
        LogError("Server application shutting down.... [REASON] socket creation failed");
        NETWORK_OPERATION_STATUS = FAILED;
        exit(-1);
    }

    if(setsockopt(SERVER_SOCKET, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int)))
    {
        LogError("Server application shutting down.... [REASON] socket option settings failed");
        NETWORK_OPERATION_STATUS = FAILED;
        exit(-1);
    }

    LogInfo("Server application has successfully created a socket");
    MAX_FD = SERVER_SOCKET;
    NETWORK_OPERATION_STATUS = SUCCEEDED;
}

void MultiplexSocket()
{
    FD_ZERO(&READ_FDS);
    FD_SET(SERVER_SOCKET, &READ_FDS);

    TIMEOUT.tv_sec = 1;
    TIMEOUT.tv_usec = 0;
}

void WaitForClients()
{

    MultiplexSocket();

    if(select(MAX_FD + 1, &READ_FDS, NULL, NULL, &TIMEOUT) < 0)
    {
        LogError("Server application shutting down.... [REASON] socket multiplexing failed");
        NETWORK_OPERATION_STATUS = FAILED;
        exit(-1);
    }

    if(FD_ISSET(SERVER_SOCKET, &READ_FDS))
    {
        struct sockaddr_in client_addr;
        socklen_t addr_len = sizeof(client_addr);

        const int client_socket = accept(SERVER_SOCKET, (struct sockaddr *) &client_addr, &addr_len);
        if(client_socket==-1)
        {
            LogError("Server application unable to establish connection with client... [REASON] Client acceptance failed");
            NETWORK_OPERATION_STATUS = FAILED;
            return ;
        }
        if (client_socket > MAX_FD)
        {
            MAX_FD = client_socket;
        }

        FD_SET(client_socket, &READ_FDS);
        LogInfo("Server application established a connection with a client. Waiting for commands...");
        NETWORK_OPERATION_STATUS = SUCCEEDED;
    }

}