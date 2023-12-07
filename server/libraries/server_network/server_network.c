#include "server_network.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <malloc.h>
#include <strings.h>
#include <unistd.h>

#include "../../../custom-libraries/myString/myString.h"
#include "../../../custom-libraries/myLogger/myLogger.h"

void InitServerSideNetwork()
{
    CreateServerSocket();

    BindServerSocket();

    OpenPortForListening();

    MultiplexSocket();
}

void OpenPortForListening()
{
    if (listen(SERVER_SOCKET, MAX_CLIENTS) == -1) {
        LogError("Server application shutting down.... [REASON] socket opening for listen failed");
        NETWORK_OPERATION_STATUS = FAILED;
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
    }
    else
    {
        char* portAsString = malloc(10);
        LogInfoFromPattern("Server application has successfully bound the socket:\n- Accepts IpV4 addresses\n- Running at port: %s\n- Accepts requests coming from any ip address",
                           IntegerToString(portAsString, PORT));
        free(portAsString);

        NETWORK_OPERATION_STATUS = SUCCEEDED;
    }
}

void CreateServerSocket()
{
    SERVER_SOCKET = socket(AF_INET, SOCK_STREAM, 0);
    if(SERVER_SOCKET == -1)
    {
        LogError("Server application shutting down.... [REASON] socket creation failed");
        NETWORK_OPERATION_STATUS = FAILED;
        return;
    }

    if(setsockopt(SERVER_SOCKET, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int)) == 0)
    {
        LogError("Server application shutting down.... [REASON] socket creation failed");
        NETWORK_OPERATION_STATUS = FAILED;
    }
    else
    {
        LogInfo("Server application has successfully created a socket");
        NETWORK_OPERATION_STATUS = SUCCEEDED;
    }
}

void MultiplexSocket()
{
    FD_ZERO(&READ_FDS);
    FD_SET(SERVER_SOCKET, &READ_FDS);

    TIMEOUT.tv_sec = 0;
    TIMEOUT.tv_usec = 0;

    MAX_FD = SERVER_SOCKET;
}

void WaitForClients()
{
    struct sockaddr_in client_addr;
    socklen_t addr_len;

    bcopy((char*) &READ_FDS, (char*) &WRITE_FDS, sizeof(READ_FDS));
    if(select(MAX_FD + 1, &READ_FDS, NULL, NULL, &TIMEOUT) == -1)
    {
        LogError("Server application shutting down.... [REASON] socket multiplexing failed");
        NETWORK_OPERATION_STATUS = FAILED;
    }

    if(FD_ISSET(SERVER_SOCKET, &READ_FDS))
    {
        addr_len = sizeof(client_addr);
        bzero(&client_addr, sizeof(client_addr));

        PrintInfo("Server application is waiting for clients...");
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