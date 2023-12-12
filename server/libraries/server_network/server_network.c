#include "server_network.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>

#include "../../../custom_libraries/custom_c_logger/custom_c_logger.h"
#include "../../../custom_libraries/custom_c_string/custom_c_string.h"

enum OperationStatus NETWORK_OPERATION_STATUS;

int SERVER_SOCKET;
int MAX_FD;
fd_set READ_FDS;
fd_set WRITE_FDS;
struct timeval TIMEOUT;

void InitServerSideNetwork()
{
    CreateServerSocket();

    BindServerSocket();

    OpenPortForListening();
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

    const char* pattern = "Server application has successfully set the socket:\n- Accepts IpV4 addresses\n- Can be found at port: %d\n- Accepts requests coming from any ip address";
    char* messageFromPattern = GetStringFromPattern(pattern, strlen(pattern) + 10, PORT);
    LogInfo(messageFromPattern);
    free(messageFromPattern);

    NETWORK_OPERATION_STATUS = SUCCEEDED;
}

void OpenPortForListening()
{
    if (listen(SERVER_SOCKET, MAX_CLIENTS) == -1) {
        LogError("Server application shutting down.... [REASON] socket opening for listen failed");
        NETWORK_OPERATION_STATUS = FAILED;
        exit(-1);
    }

    const char* pattern = "Server application has successfully opened the socket for listening on port %d";
    char* messageFromPattern = GetStringFromPattern(pattern, strlen(pattern) + 10, PORT);
    LogInfo(messageFromPattern);
    free(messageFromPattern);
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
            LogError("Server application unable to establish connection with new client... [REASON] Client acceptance failed");
            NETWORK_OPERATION_STATUS = FAILED;
            return ;
        }
        if (client_socket > MAX_FD)
        {
            MAX_FD = client_socket;
        }

        FD_SET(client_socket, &READ_FDS);
        const char* pattern = "Server application found a new client at fd: %d.";
        char* messageFromPattern = GetStringFromPattern(pattern, strlen(pattern) + 10, client_socket);
        LogInfo(messageFromPattern);
        free(messageFromPattern);
        NETWORK_OPERATION_STATUS = SUCCEEDED;
    }

}